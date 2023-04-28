#include "../libs/easy-bluetooth/btmanager.h"
#include "../libs/easy-bluetooth/btscanner.h"
#include "../libs/easy-bluetooth/btreceiver.h"
#include "../libs/easy-bluetooth/bluetooth_services_model.h"
#include "../usecases/client_actions.h"
#include "../usecases/device_store.h"
#include "bt_store.h"
#include <QQmlEngine>
#include <QMetaType>
#include <QScxmlStateMachine>


BtStore::BtStore(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<BtReceiver*>("BtReceiver");
    qmlRegisterUncreatableMetaObject(btreceiverstatus::staticMetaObject, "com.fpz.blowermonitor", 1, 0, "BtReceiverStatus", "Access to enums & flags only");
    m_manager = new BtManager(this);
    m_scanner = new BtScanner(this);
    m_receiver = new BtReceiver(m_manager, QSharedPointer<AbstractPacketParser>(
                                    new JsonObjectPacketParser), this);

   m_availableDevicesModel = new BluetoothServicesModel(this);
   m_savedDevicesModel = new SavedBluetoothServicesModel(this);
   connect(m_scanner, &BtScanner::deviceDiscovered, this, &BtStore::deviceDiscovered);
}
::
btreceiverstatus::Status BtStore::receiverStatus() const
{
    return m_receiverStatus;
}

BtReceiver* BtStore::receiver()
{
    return m_receiver;
}

BtScanner* BtStore::scanner()
{
    return m_scanner;
}

BtManager *BtStore::manager()
{
    return m_manager;
}

void BtStore::setReceiverStatus(btreceiverstatus::Status val)
{
    if (m_receiverStatus == val)
        return;

    m_receiverStatus = val;
    emit receiverStatusChanged(m_receiverStatus);
}

BluetoothServicesModel *BtStore::availableDevicesModel() const
{
    return m_availableDevicesModel;
}

SavedBluetoothServicesModel *BtStore::savedDevicesModel() const
{
    return m_savedDevicesModel;
}

void BtStore::setCurrentServiceInfo(const QBluetoothServiceInfo &val)
{
    m_currentServiceInfo = val;
}

const QBluetoothServiceInfo &BtStore::currentServiceInfo()
{
    return m_currentServiceInfo;
}
