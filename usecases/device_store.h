#ifndef DEVICE_STORE_H
#define DEVICE_STORE_H

#include <QObject>
#include <QTimer>

namespace sensordatastatus {
Q_NAMESPACE;
enum class Status {
    UNKNOWN,
    REGULAR,
    OLD,
    TOO_OLD
};
Q_ENUM_NS(Status);
}

class DeviceStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentDeviceName READ currentDeviceName NOTIFY currentDeviceNameChanged)
    Q_PROPERTY(QString deviceNameFilter READ deviceNameFilter NOTIFY discoverDeviceNameChanged)
    Q_PROPERTY(QString serviceUuid READ serviceUuid NOTIFY serviceUuidChanged)
    Q_PROPERTY(int sensorDataLastUpdated READ sensorDataLastUpdated NOTIFY sensorDataLastUpdatedChanged)
    Q_PROPERTY(sensordatastatus::Status sensorDataStatus READ sensorDataStatus NOTIFY sensorDataStatusChanged)
    Q_PROPERTY(double channel1Value READ channel1Value NOTIFY channel1ValueChanged)
    Q_PROPERTY(double channel2Value READ channel2Value NOTIFY channel2ValueChanged)
    Q_PROPERTY(double channel3Value READ channel3Value NOTIFY channel3ValueChanged)
    Q_PROPERTY(double channel4Value READ channel4Value NOTIFY channel4ValueChanged)
    Q_PROPERTY(double channel5Value READ channel5Value NOTIFY channel5ValueChanged)
    Q_PROPERTY(double channel6Value READ channel6Value NOTIFY channel6ValueChanged)

public:
    explicit DeviceStore(QObject *parent = nullptr);

    const QString& currentDeviceName() const;
    const QString& deviceNameFilter() const;
    int currentDeviceIdx() const;
    int sensorDataLastUpdated() const;
    sensordatastatus::Status sensorDataStatus() const;
    const QString& serviceUuid() const;

    void setCurrentDeviceName(const QString& currentDeviceName);
    void setDiscoverDeviceName(const QString& deviceName);
    void setCurrentDeviceIdx(int currentDeviceIdx);
    void setSensorDataLastUpdated(int secs);
    void setSensorDataStatus(sensordatastatus::Status sensorDataStatus);
    void setServiceUuid(const QString& serviceUuid);

    void startDataLastUpdatedInterval();
    void stopDataLastUpdatedInterval();

    double channel1Value() const;
    void setChannel1Value(double newChannel1Value);

    double channel2Value() const;
    void setChannel2Value(double newChannel2Value);

    double channel3Value() const;
    void setChannel3Value(double newChannel3Value);

    double channel4Value() const;
    void setChannel4Value(double newChannel4Value);

    double channel5Value() const;
    void setChannel5Value(double newChannel5Value);

    double channel6Value() const;
    void setChannel6Value(double newChannel6Value);

signals:
    void currentDeviceNameChanged(QString currentDeviceName);
    void discoverDeviceNameChanged(QString deviceName);
    void currentDeviceIdxChanged(int currentDeviceIdx);
    void sensorDataLastUpdatedChanged(int secs);
    void sensorDataStatusChanged(sensordatastatus::Status sensorDataStatus);
    void serviceUuidChanged(QString serviceUuid);

    void channel1ValueChanged();
    void channel2ValueChanged();
    void channel3ValueChanged();
    void channel4ValueChanged();
    void channel5ValueChanged();
    void channel6ValueChanged();

private:
    QString m_currentDeviceName;
    QString m_discoverDeviceName;
    int m_currentDeviceIdx{-1};
    int m_sensorDataLastUpdated{};
    QTimer* m_sensorDataLastUpdatedTimer;
    sensordatastatus::Status m_sensorDataStatus;
    QString m_serviceUuid;
    double m_channel1Value {0};
    double m_channel2Value {0};
    double m_channel3Value {0};
    double m_channel4Value {0};
    double m_channel5Value {0};
    double m_channel6Value {0};
};

#endif // DEVICE_STORE_H
