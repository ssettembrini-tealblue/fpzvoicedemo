#ifndef READCURRENTPARAMS_H
#define READCURRENTPARAMS_H

#include <QObject>
#include "client_actions.h"
#include "device_store.h"
#include "bt_store.h"

class ReadCurrentParams : public QObject
{
    Q_OBJECT
public:
    explicit ReadCurrentParams(ClientActions* clientActions, BtStore* btStore, DeviceStore *deviceStore, QObject *parent);

private:
    DeviceStore* m_deviceStore;
    BtStore* m_btStore;
    ClientActions* m_clientActions;

};

#endif // READCURRENTPARAMS_H
