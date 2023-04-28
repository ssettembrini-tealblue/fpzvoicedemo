#ifndef BTRECEIVER_H
#define BTRECEIVER_H


#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QObject>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QDebug>
#include <QLatin1String>
#include <qbluetoothuuid.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QQueue>
#include <QFile>
#include <QPointer>
#include "btmanager.h"

struct AbstractPacketParser {
    virtual void clear() = 0;
    virtual QList<QByteArray> parseMessages(const QByteArray &data) = 0;
};

struct JsonObjectPacketParser: public AbstractPacketParser {
    void clear() override {
        parsed.clear();
        curlyNesting = 0;
    }
    QList<QByteArray> parseMessages(const QByteArray &data) override {
        QList<QByteArray> res;
        for (int i = 0; i < data.size(); ++i) {
            const char b = data.at(i);
            switch (b) {
            case '{' : {
                parsed.append(b);
                ++curlyNesting;
                break;
            }
            case '}' : {
                if (curlyNesting > 0) {
                    parsed.append(b);
                    --curlyNesting;
                    if (curlyNesting == 0) {
                        res.push_back(parsed);
                        parsed.clear();
                    }
                }
                // else discard
                break;
            }
            default: {
                if (curlyNesting > 0) // else discard
                    parsed.append(b);
                break;
            }
            }
        }
        return res;
    };

    QByteArray parsed;
    int curlyNesting = 0;
};

class BtReceiver : public QObject
{
//#define BTRECEIVER_DUMPTOFILE 1
    Q_OBJECT
public:

    BtReceiver(BtManager *pBtManager,
               const QSharedPointer<AbstractPacketParser> &parser_,
               QObject *parent = 0) :
        QObject(parent)
      ,m_pBtManager(pBtManager)
      ,socket(0)
      ,parser(parser_)
    {
    }

    ~BtReceiver()
    {

    }

    // to avoid exposing QBluetoothServiceInfo, call this from scanner class
    bool startReceiver(QBluetoothServiceInfo serviceInfo, bool force=false)
    {

        if (!m_pBtManager->isBluetoothEnabled()) {
            qDebug() << "Bluetooth is NOT Enabled!!";
            return false;
        }
        if (!serviceInfo.isValid()) {
            qDebug() << "No Valid serviceInfo to connect to!!";
            return false;
        }

        if (force) {
            if (socket) {
                disconnect(socket);
                socket->deleteLater();
            }
            socket = NULL;
        }
        if (socket)  {
            qDebug() << "Already connected?";
            return true;
        }

        // Connect to service
        socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
        socket->connectToService(serviceInfo);
        qDebug() << "ConnecttoService done";

        connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
        connect(socket, SIGNAL(connected()), this, SLOT(connected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(stopReceiver()));
        connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(errorOccurred(QBluetoothSocket::SocketError)));
        return true;
    }

    // Some logic that tries to reconnect to the same device is pretty much needed
    Q_INVOKABLE void stopReceiver()
    {
        parser.clear();
        if (socket) {
            qDebug() << "btreceiver: stopping receiving";
            disconnect(socket);
            socket->deleteLater();
            socket = nullptr;
            emit statusChanged(false);
        }
    }

    Q_INVOKABLE bool isConnected()
    {
        return (socket);
    }

    QBluetoothServiceInfo currentService() const // for error reporting
    {
        return m_currentService;
    }

signals:
    void packet(const QByteArray data);
    void statusChanged(bool connected);
    void error(QBluetoothSocket::SocketError, const QString &err);

public slots:

    void connected() {
        qDebug() << "The device is now connected via Bluetooth";
        emit statusChanged(true);
    }

    void errorOccurred(QBluetoothSocket::SocketError error) {
        QString err;
        switch (error)
        {
        qDebug() << "QBluetoothSocket error";
        case QBluetoothSocket::UnknownSocketError:
            err = "An unknown error has occurred.";
            break;
        case QBluetoothSocket::NoSocketError:
            err = "No error. Used for testing.";
            break;
        case QBluetoothSocket::UnsupportedProtocolError:
            err = "Unsupported protocol";
            break;
        case QBluetoothSocket::RemoteHostClosedError:
            err = "The remote host closed the socket";
            break;
        case QBluetoothSocket::HostNotFoundError:
            err = "Could not find the remote host";
            break;
        case QBluetoothSocket::ServiceNotFoundError:
            err = "Could not find the service UUID on remote host";
            break;
        case QBluetoothSocket::OperationError	:
            err = "Operation error";
            break;
        case QBluetoothSocket::NetworkError	:
            err = "Attempt to read or write from socket returned an error";
            break;
        }
        qDebug() << err;
        if (error != QBluetoothSocket::NoSocketError) {
            emit BtReceiver::error(error, err);
            stopReceiver();
        }
    }

    void readSocket()
    {
        if (!socket) return;
        QByteArray readData = socket->readAll();
        QList<QByteArray> packets = parser->parseMessages(readData);
        for (const auto &p: packets)
            emit packet(p);
    }

private:

    QBluetoothServiceInfo m_currentService; // used for error reporting
    BtManager *m_pBtManager;
    QPointer<QBluetoothSocket> socket;
    QSharedPointer<AbstractPacketParser> parser;
};

#endif // BTRECEIVER_H
