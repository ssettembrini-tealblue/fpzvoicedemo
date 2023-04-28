#ifndef BTSTORE_H
#define BTSTORE_H
#include <memory>
#include <QObject>
#include "../libs/easy-bluetooth/btscanner.h"
#include "../libs/easy-bluetooth/btreceiver.h"
#include "client_actions.h"
#include <bluetooth_services_model.h>

Q_DECLARE_METATYPE(BtReceiver*)

namespace btreceiverstatus {
Q_NAMESPACE;
enum class Status {
    UNKNOWN,
    DISCONNECTED,
    SEARCHING,
    CONNECTED,
    DISABLED
};
Q_ENUM_NS(Status);
}

class BtStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(btreceiverstatus::Status receiverStatus READ receiverStatus NOTIFY receiverStatusChanged)
    Q_PROPERTY(BtReceiver* receiver MEMBER m_receiver)
    Q_PROPERTY(BluetoothServicesModel* availableDevicesModel READ availableDevicesModel)
    Q_PROPERTY(SavedBluetoothServicesModel* savedDevicesModel READ savedDevicesModel)


public:
    explicit BtStore(QObject* parent=nullptr);
    btreceiverstatus::Status receiverStatus() const;
    BtReceiver* receiver();
    BtScanner* scanner();
    BtManager* manager();
    void setReceiverStatus(btreceiverstatus::Status);
    BluetoothServicesModel *availableDevicesModel() const;
    SavedBluetoothServicesModel *savedDevicesModel() const;
    void setCurrentServiceInfo(const QBluetoothServiceInfo&);
    const QBluetoothServiceInfo& currentServiceInfo();
signals:
    void receiverStatusChanged(btreceiverstatus::Status receiverStatus);
    void deviceDiscovered(const QString& deviceName);
private:
    btreceiverstatus::Status m_receiverStatus{};
    BtManager* m_manager;
    BtReceiver* m_receiver;
    BtScanner* m_scanner;

    BluetoothServicesModel* m_availableDevicesModel;
    SavedBluetoothServicesModel* m_savedDevicesModel;

    QBluetoothServiceInfo m_currentServiceInfo{};
};

#endif // BTSTORE_H
