#pragma once
#ifndef BTSERVER_H
#define BTSERVER_H

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QObject>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include <QtBluetooth/qbluetoothserver.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QDebug>
#include <QLatin1String>
#include <qbluetoothuuid.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QQueue>
#include <QFile>
#include <QPointer>


// ToDo: modularize this class

namespace  {
// random UUID to uniquely describe this service.
const QLatin1String serviceUuid("00001101-0000-1000-8000-00805f9b34fb");//e8e10f95-1a70-4b27-9ccf-02010264e9c3");
}

class BtServer : public QObject
{
    Q_OBJECT
public:
    BtServer(QObject *parent = nullptr) : QObject(parent) {

    }
    ~BtServer() override {

    }

    Q_INVOKABLE void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress())
    {
        if (rfcommServer)
            return;
#ifdef Q_OS_ANDROID
        QBluetoothLocalDevice localDevice;
        if (localDevice.isValid()) {
            localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        }
#endif
        rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
        rfcommServer->setSecurityFlags(QBluetooth::Authentication);
        connect(rfcommServer, &QBluetoothServer::newConnection,
                this, QOverload<>::of(&BtServer::clientConnected));
#if 1 // "listen(localAdapter)" replaces the manual setup contained in the else block
        rfcommServer->listen(QBluetoothUuid(serviceUuid), "FPZ TEST");
#else
        //! [Create the server]

        bool result = rfcommServer->listen(localAdapter);
        if (!result) {
            qWarning() << "Cannot bind bt server to" << localAdapter.toString();
            return;
        }
        qDebug() << "Starting service: " << QBluetoothUuid(serviceUuid);

        // The server is only useful if others know that it is there.
        // To enable other devices to discover it, a record describing the service needs to be published
        // in the systems SDP (Service Discovery Protocol) database. The QBluetoothServiceInfo class encapsulates
        // a service record.
        // We will publish a service record that contains some textural descriptions of the services,
        // a UUID that uniquely identifies the service, the discoverability attribute, and connection parameters.

        QBluetoothServiceInfo::Sequence profileSequence;
        QBluetoothServiceInfo::Sequence classId;
        classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
        classId << QVariant::fromValue(quint16(0x100));
        profileSequence.append(QVariant::fromValue(classId));
        serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                                 profileSequence);

        classId.clear();
        classId << QVariant::fromValue(QBluetoothUuid(serviceUuid));
        classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

        // The textural description of the service is stored in the ServiceName,
        // ServiceDescription, and ServiceProvider attributes.
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, QLatin1String("Bt FPZ Test Server"));
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                                 QLatin1String("Bt FPZ Test Server"));
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, QLatin1String("tbd.org"));


        // ToDo: find out why these info are not received by android
        serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));
        serviceInfo.setServiceName(QLatin1String("Bt FPZ Test Server"));
        serviceInfo.setServiceDescription(QLatin1String("Bt FPZ Test Server"));
        serviceInfo.setServiceProvider(QLatin1String("tbd.org"));
        qDebug() << "Starting service2: " << serviceInfo.serviceUuid();

        // A Bluetooth service is only discoverable if it is in the PublicBrowseGroup.
        QBluetoothServiceInfo::Sequence publicBrowse;
        publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
        serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                                 publicBrowse);

        // The ProtocolDescriptorList attribute is used to publish the connection parameters
        // that the remote device requires to connect to our service.
        // Here we specify that the Rfcomm protocol is used and set the port number to the port
        // that our rfcommServer instance is listening to.
        QBluetoothServiceInfo::Sequence protocolDescriptorList;
        QBluetoothServiceInfo::Sequence protocol;
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
                 << QVariant::fromValue(quint8(rfcommServer->serverPort()));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                                 protocolDescriptorList);


        // Finally, we register the service record with the system.
        serviceInfo.registerService(localAdapter);
#endif
    }

    Q_INVOKABLE void stopServer() {
        // Unregister service
        serviceInfo.unregisterService();

        // Close sockets
        qDeleteAll(clientSockets);

        // Close server
        delete rfcommServer;
        rfcommServer = nullptr;
    }

public slots:
    void send(const QByteArray &data) {
        for (QBluetoothSocket *socket : qAsConst(clientSockets))
            socket->write(data);
    }

signals:
    void messageReceived(const QString &sender, const QString &message);
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);

private slots:
    void clientConnected() {
        QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
        if (!socket)
            return;

        connect(socket, &QBluetoothSocket::readyRead, this, &BtServer::readSocket);
        connect(socket, &QBluetoothSocket::disconnected, this, QOverload<>::of(&BtServer::clientDisconnected));
        clientSockets.append(socket);
        emit clientConnected(socket->peerName());
    }
    void clientDisconnected() {
        QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
        if (!socket)
            return;

        emit clientDisconnected(socket->peerName());

        clientSockets.removeOne(socket);

        socket->deleteLater();
    }
    void readSocket() {
        QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
        if (!socket)
            return;

        QByteArray data = socket->readAll();
        // ToDo: parse this through a custom parser and emit the list of packets the parser returns
    }

private:
    QBluetoothServer *rfcommServer = nullptr;
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;
};

#endif // BTSERVER_H
