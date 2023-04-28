#include <QScxmlStateMachine>
#include <QIODevice>
#include <QTextStream>
#include "connect_device_via_bt.h"

ConnectDeviceViaBt::ConnectDeviceViaBt(ClientActions* clientActions, BtStore* btStore, DeviceStore *deviceStore, QObject *parent) : QObject(parent)
{
    m_deviceStore = deviceStore;
    m_btStore = btStore;
    m_machine = QScxmlStateMachine::fromFile(
                QStringLiteral(":ConnectDeviceViaBt.scxml"));
    if (!m_machine->parseErrors().isEmpty()) {
        QTextStream errs(stderr, QIODevice::WriteOnly);
        const auto errors = m_machine->parseErrors();
        for (const QScxmlError &error : errors) {
            errs << error.toString();
        }
    }
    m_machine->setParent(parent);

    QObject::connect(m_machine, &QScxmlStateMachine::reachedStableState, parent, [this](){
        qDebug() << m_machine->activeStateNames(true).at(0);
    });

    /* --- activateBt --- */

    QObject::connect(m_btStore->manager(), &BtManager::bluetoothEnabled, this, [this]() {
        m_machine->submitEvent(QStringLiteral("outcome.btReceiver_active"));
    });

    QObject::connect(m_btStore->manager(), &BtManager::errorEnablingBluetooth, this, [this]() {
        m_machine->submitEvent(QStringLiteral("outcome.btReceiver_cantActivate"));
    });

    m_machine->connectToState(QStringLiteral("activatingBt"), this, [this](bool active) { // active means we are entering this state, not leaving it
        if (active) {
            qDebug() << "activating bluetooth...";
            if (!m_btStore->manager()->isBluetoothEnabled()) {
                m_btStore->manager()->toggleBluetooth(true);
                return;
            }
            m_machine->submitEvent(QStringLiteral("outcome.btReceiver_active"));
        }
    });

    /* --- checkForKnownDevices --- */

    m_machine->connectToState(QStringLiteral("checkForKnownDevices"), [this](bool active) {
        if (active) {
            qDebug() << "checkForKnownDevices";
            m_btStore->savedDevicesModel()->rowCount() == 0
                    ? m_machine->submitEvent(QStringLiteral("outcome.knownDevices_noneFound"))
                    : m_machine->submitEvent(QStringLiteral("outcome.knownDevices_someFound"));
        }
    });

    /* --- defineConnectionPolicy --- */

    // HANDLED IN UI

    /* --- checkForOneOrMoreDevices --- */

    m_machine->connectToState(QStringLiteral("checkForOneOrMoreDevices"), [this](bool active) {
        if (active) {
            if (m_btStore->savedDevicesModel()->rowCount() == 1) {
                auto serviceInfo = m_btStore->savedDevicesModel()->serviceInfo(0);
                qDebug() << "saved info:" << serviceInfo.device().name() << serviceInfo.serviceUuid();
                m_btStore->setCurrentServiceInfo(serviceInfo);
                m_machine->submitEvent(QStringLiteral("outcome.oneDevice_known"));
            } else {
                m_machine->submitEvent(QStringLiteral("outcome.moreDevices_known"));
            }
        }
    });

    /* --- discoverAvailableDevices --- */

    QObject::connect(clientActions, &ClientActions::startBtDeviceDiscovery, this,
                     [this](const QString& deviceName, const QString& serviceUuid){
        m_deviceStore->setDiscoverDeviceName(deviceName);
        m_deviceStore->setServiceUuid(serviceUuid);
        m_machine->start();
    });

    QObject::connect(clientActions, &ClientActions::stopBtDeviceDiscovery, this,
                     [this](){
        m_btStore->scanner()->stopDeviceDiscovery();
    });

    QObject::connect(m_btStore->scanner(), &BtScanner::deviceDiscoveryIsFinished, this, [this](int cnt){
        if (!cnt || !m_btStore->scanner()->devicesCount()) {
            m_btStore->availableDevicesModel()->clear();
            m_machine->submitEvent(QStringLiteral("outcome.devices_noneFound"));
        } else {
            // update the model
            QVariantList availableDevices;
            for (int i = 0; i < cnt; ++i) {
                QVariantMap service;
                service["deviceName"] = m_btStore->scanner()->getDeviceName(i);
                availableDevices << service;
                qDebug() << "=== NAME: " << service["deviceName"] <<  " address: " << m_btStore->scanner()->getDeviceInfo(i).address().toString();
            }
            m_btStore->availableDevicesModel()->updateData(availableDevices);
            if (!availableDevices.size()) {
                m_machine->submitEvent(QStringLiteral("outcome.devices_noneFound"));
                return;
            } else {
                for (int i=0; i < m_btStore->availableDevicesModel()->rowCount(); ++i) {
                    auto serviceInfo = m_btStore->scanner()->infoFromDevice(i, m_deviceStore->serviceUuid());
                    m_btStore->savedDevicesModel()->addService(serviceInfo);
                }
            }
            m_btStore->savedDevicesModel()->sync();
            if (availableDevices.size() == 1) {
                auto serviceInfo = m_btStore->scanner()->infoFromDevice(0, m_deviceStore->serviceUuid());
                m_btStore->setCurrentServiceInfo(serviceInfo);
                m_machine->submitEvent(QStringLiteral("outcome.devices_oneFound"));
            } else {
                m_machine->submitEvent(QStringLiteral("outcome.devices_someFound"));
            }
        }
    });

    QObject::connect( m_btStore->scanner(), &BtScanner::deviceDiscoveryError, this, [this](const QString /*error*/){
        // FixMe
        //            connectDeviceViaBtSM->submitEvent(QStringLiteral("<some error event>"));
    });

    m_machine->connectToState(QStringLiteral("discoverAvailableDevices"), this, [this](bool active) {
        if (active) {
            m_btStore->setReceiverStatus(btreceiverstatus::Status::SEARCHING);
            m_btStore->scanner()->startDeviceDiscoveryByName(m_deviceStore->deviceNameFilter());
        }
    });

    /* --- selectNewDeviceToConnect --- */

    // this state is explicited in the UI, and the outcome would be
    // a BluetoothServicesModel::deviceSelected signal
    QObject::connect(clientActions, &ClientActions::selectNewDevice, this, [this](const QString& deviceName) {
        // Store this device into saved devices
        auto serviceInfo = m_btStore->savedDevicesModel()->serviceInfo(deviceName, m_deviceStore->serviceUuid());
        qDebug() << "saved info:" << serviceInfo.device().name() << serviceInfo.serviceUuid();
        m_btStore->setCurrentServiceInfo(serviceInfo);
        m_machine->submitEvent(QStringLiteral("outcome.newDeviceToConnect_selected"));
    });

    /* --- selectKnownDeviceToConnect --- */

    QObject::connect(clientActions, &ClientActions::selectKnownDevice, this, [this](const QString& deviceName) {
        auto serviceInfo = m_btStore->savedDevicesModel()->serviceInfo(deviceName, m_deviceStore->serviceUuid());
        m_btStore->setCurrentServiceInfo(serviceInfo);
        m_machine->submitEvent(QStringLiteral("outcome.knownDeviceToConnect_selected"));
    });

    /* --- connectDevice --- */

    QObject::connect(m_btStore->receiver(), &BtReceiver::error, this, [this](QBluetoothSocket::SocketError error, const QString err) {
        // ToDo: consider sending this error to the UI
        qWarning() << "Error connecting to service at "
                   << m_btStore->receiver()->currentService().device().name() << " : "
                   << err;
        switch (error) {
        case QBluetoothSocket::NetworkError	: m_machine->submitEvent(QStringLiteral("outcome.device_disconnected")); break;
        default:;
        }
    });

    m_machine->connectToState(QStringLiteral("connectingDevice"), m_machine,
                              [this](bool active) {
        if (active) {
            if (m_btStore->currentServiceInfo().isValid()) {
                qDebug() << "======= Starting receiver with constructed service info!!";
                m_btStore->receiver()->startReceiver(m_btStore->currentServiceInfo());
            } else {
                m_machine->submitEvent(QStringLiteral("outcome.device_cantConnect"));
            }
        }
    });

    QObject::connect(m_btStore->receiver(), &BtReceiver::statusChanged, this, [this](bool connected) {
        if (connected) {
            // all good, BtReceiver::packet signals will begin shortly
            m_btStore->setReceiverStatus(btreceiverstatus::Status::CONNECTED);
            // Add current service to list of saved services
            m_machine->submitEvent(QStringLiteral("outcome.device_connected"));
        } else {
            qWarning() << "Failed connecting to service at "
                       << m_btStore->receiver()->currentService().device().name();
            m_machine->submitEvent(QStringLiteral("outcome.device_cantConnect"));
        }
    });

    QObject::connect(m_btStore->receiver(), &BtReceiver::packet, this, [this](const QByteArray data){
        qDebug() << data;
        m_deviceStore->startDataLastUpdatedInterval();
    });

    /* --- deviceDisconnected --- */

    m_machine->connectToState(QStringLiteral("deviceDisconnected"), this, [this](bool active) {
        if (active) {
            m_btStore->setReceiverStatus(btreceiverstatus::Status::DISCONNECTED);
            m_deviceStore->stopDataLastUpdatedInterval();
        }
    });
}

QScxmlStateMachine *ConnectDeviceViaBt::machine() const
{
    return m_machine;
}
