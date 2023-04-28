#ifndef CONNECTDEVICEVIABT_H
#define CONNECTDEVICEVIABT_H

#include <QObject>
#include "client_actions.h"
#include "device_store.h"
#include "bt_store.h"
class QScxmlStateMachine;
class ConnectDeviceViaBt : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QScxmlStateMachine* machine READ machine)
public:
    explicit ConnectDeviceViaBt(ClientActions* clientActions, BtStore* btStore, DeviceStore* deviceStore, QObject *parent = nullptr);

    QScxmlStateMachine *machine() const;

private:
    QScxmlStateMachine* m_machine;
    DeviceStore* m_deviceStore;
    BtStore* m_btStore;

};

#endif // CONNECTDEVICEVIABT_H
