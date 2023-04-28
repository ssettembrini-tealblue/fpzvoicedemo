#ifndef BTMANAGER_H
#define BTMANAGER_H

#include <QObject>
#include <QTimer>
#include <QBluetoothLocalDevice>
#include <QDebug>


class BtManager : public QObject
 {
     Q_OBJECT
 public:

    BtManager(QObject *parent = 0) : QObject(parent)
      ,m_btLocalDevice(this)
      ,m_timerBtChecker(this)
      ,m_bBluetoothEnabled(false)
      ,m_bBluetoothAtStart(isBluetoothEnabled())
    {
        toggleBluetooth(true);
        qDebug() << "BtManager created\n";
#if 0
        QTimer::singleShot(3000, this, SLOT(checkBluetoothF()));
        connect(&m_timerBtChecker, SIGNAL(timeout()), this, SLOT(checkBluetooth()));
        m_timerBtChecker.setInterval(15000);
        m_timerBtChecker.start();  // The point was to handle users switching off bt while using app
#endif
    }

    ~BtManager() override
    {
        if (m_bBluetoothAtStart)
            m_btLocalDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        else
            m_btLocalDevice.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    }

    Q_INVOKABLE bool isBluetoothEnabled() {
        if (m_btLocalDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff) return false;
        else return true;
    }

    Q_INVOKABLE void toggleBluetooth(bool enabled) {
        if (enabled) enableBluetooth();
        else disableBluetooth();
    }

    Q_INVOKABLE void enableBluetooth() {
        m_bBluetoothEnabled = true;
        if (m_btLocalDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
            m_btLocalDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
            if (m_btLocalDevice.hostMode() != QBluetoothLocalDevice::HostDiscoverable)
                emit errorEnablingBluetooth();
            else
                emit bluetoothEnabled();
        }

    }
    Q_INVOKABLE void disableBluetooth() {
        m_bBluetoothEnabled = false;
        if (m_btLocalDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff) {
            m_btLocalDevice.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
            if (m_btLocalDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff)
                emit errorDisablingBluetooth();
            else
                emit bluetoothDisabled();
        }
    }



signals:
    void bluetoothEnabled();
    void bluetoothDisabled();
    void errorEnablingBluetooth();
    void errorDisablingBluetooth();

#if 0
private slots:
    Q_INVOKABLE void checkBluetoothF() {
        if (isBluetoothEnabled()) {
            m_bBluetoothEnabled = true;
            emit bluetoothEnabled();
        }
    }
    Q_INVOKABLE void checkBluetooth() {
        if (isBluetoothEnabled()) {
            if (!m_bBluetoothEnabled) {
                m_bBluetoothEnabled = true;
                emit bluetoothEnabled();
            }
        } else {
            if (m_bBluetoothEnabled) {
                m_bBluetoothEnabled = false;
                emit bluetoothDisabled();
            }
        }
    }
#endif

private:
    QBluetoothLocalDevice m_btLocalDevice;
    QTimer m_timerBtChecker;

    bool m_bBluetoothEnabled;
    bool m_bBluetoothAtStart;
};

#endif // BTMANAGER_H
