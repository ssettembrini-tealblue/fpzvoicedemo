#include "device_store.h"
#include <QQmlEngine>

DeviceStore::DeviceStore(QObject *parent)
    : QObject(parent)
{
    qmlRegisterUncreatableMetaObject(sensordatastatus::staticMetaObject, "com.fpz.blowermonitor", 1, 0, "SensorDataStatus", "Access to enums & flags only");
    m_sensorDataLastUpdatedTimer = new QTimer(this);
    m_sensorDataLastUpdatedTimer->setInterval(1000);
    connect(m_sensorDataLastUpdatedTimer, &QTimer::timeout, this, [this]{
        setSensorDataLastUpdated(sensorDataLastUpdated() + 1);
        if (sensorDataLastUpdated() > 15) {
            setSensorDataStatus(sensordatastatus::Status::TOO_OLD);
        } else if (sensorDataLastUpdated() > 8) {
            setSensorDataStatus(sensordatastatus::Status::OLD);
        } else {
            setSensorDataStatus(sensordatastatus::Status::REGULAR);
        }
    });
}

const QString& DeviceStore::currentDeviceName() const
{
    return m_currentDeviceName;
}

const QString& DeviceStore::deviceNameFilter() const
{
    return m_discoverDeviceName;
}

int DeviceStore::currentDeviceIdx() const
{
    return m_currentDeviceIdx;
}

int DeviceStore::sensorDataLastUpdated() const
{
    return m_sensorDataLastUpdated;
}

void DeviceStore::setCurrentDeviceName(const QString& currentDeviceName)
{
    if (m_currentDeviceName == currentDeviceName)
        return;

    m_currentDeviceName = currentDeviceName;
    emit currentDeviceNameChanged(m_currentDeviceName);
}

void DeviceStore::setDiscoverDeviceName(const QString& deviceName)
{
    if (m_discoverDeviceName == deviceName)
        return;

    m_discoverDeviceName = deviceName;
    emit discoverDeviceNameChanged(m_discoverDeviceName);
}

void DeviceStore::setCurrentDeviceIdx(int currentDeviceIdx)
{
    if (m_currentDeviceIdx == currentDeviceIdx)
        return;

    m_currentDeviceIdx = currentDeviceIdx;
    emit currentDeviceIdxChanged(m_currentDeviceIdx);
}

void DeviceStore::setSensorDataLastUpdated(int secs)
{
    if (m_sensorDataLastUpdated == secs)
        return;

    m_sensorDataLastUpdated = secs;
    emit sensorDataLastUpdatedChanged(m_sensorDataLastUpdated);
}

void DeviceStore::startDataLastUpdatedInterval()
{
    setSensorDataLastUpdated(0);
    setSensorDataStatus(sensordatastatus::Status::REGULAR);
    m_sensorDataLastUpdatedTimer->start();
}

void DeviceStore::stopDataLastUpdatedInterval()
{
    m_sensorDataLastUpdatedTimer->stop();
    setSensorDataStatus(sensordatastatus::Status::UNKNOWN);
    setSensorDataLastUpdated(0);
}

void DeviceStore::setServiceUuid(const QString &serviceUuid)
{
    if (m_serviceUuid == serviceUuid)
        return;

    m_serviceUuid = serviceUuid;
    emit serviceUuidChanged(m_serviceUuid);
}

const QString& DeviceStore::serviceUuid() const
{
    return m_serviceUuid;
}

sensordatastatus::Status DeviceStore::sensorDataStatus() const
{
    return m_sensorDataStatus;
}

void DeviceStore::setSensorDataStatus(sensordatastatus::Status sensorDataStatus)
{
    if (m_sensorDataStatus == sensorDataStatus)
        return;

    m_sensorDataStatus = sensorDataStatus;
    emit sensorDataStatusChanged(m_sensorDataStatus);
}

double DeviceStore::channel1Value() const
{
    return m_channel1Value;
}

void DeviceStore::setChannel1Value(double newChannel1Value)
{
    if (qFuzzyCompare(m_channel1Value, newChannel1Value))
        return;
    m_channel1Value = newChannel1Value;
    emit channel1ValueChanged();
}

double DeviceStore::channel2Value() const
{
    return m_channel2Value;
}

void DeviceStore::setChannel2Value(double newChannel2Value)
{
    if (qFuzzyCompare(m_channel2Value, newChannel2Value))
        return;
    m_channel2Value = newChannel2Value;
    emit channel2ValueChanged();
}

double DeviceStore::channel3Value() const
{
    return m_channel3Value;
}

void DeviceStore::setChannel3Value(double newChannel3Value)
{
    if (qFuzzyCompare(m_channel3Value, newChannel3Value))
        return;
    m_channel3Value = newChannel3Value;
    emit channel3ValueChanged();
}

double DeviceStore::channel4Value() const
{
    return m_channel4Value;
}

void DeviceStore::setChannel4Value(double newChannel4Value)
{
    if (qFuzzyCompare(m_channel4Value, newChannel4Value))
        return;
    m_channel4Value = newChannel4Value;
    emit channel4ValueChanged();
}

double DeviceStore::channel5Value() const
{
    return m_channel5Value;
}

void DeviceStore::setChannel5Value(double newChannel5Value)
{
    if (qFuzzyCompare(m_channel5Value, newChannel5Value))
        return;
    m_channel5Value = newChannel5Value;
    emit channel5ValueChanged();
}

double DeviceStore::channel6Value() const
{
    return m_channel6Value;
}

void DeviceStore::setChannel6Value(double newChannel6Value)
{
    if (qFuzzyCompare(m_channel6Value, newChannel6Value))
        return;
    m_channel6Value = newChannel6Value;
    emit channel6ValueChanged();
}
