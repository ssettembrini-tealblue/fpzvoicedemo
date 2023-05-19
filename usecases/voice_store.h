#ifndef VOICE_STORE_H
#define VOICE_STORE_H

#include <QObject>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QWebSocket>
#include "client_actions.h"

enum class TypeMsg
{
    Start = 1,
    Stop = 2,
    Set = 3,
    Increase = 4,
    Decrease = 5,
    Unknown = 0
};


class VoiceStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString connectionIp READ connectionIp WRITE setConnectionIp NOTIFY connectionIpChanged)
    Q_PROPERTY(int connectionPort READ connectionPort WRITE setConnectionPort NOTIFY connectionPortChanged)
    Q_PROPERTY(QString connectionAddress READ connectionAddress WRITE setConnectionAddress NOTIFY connectionAddressChanged)
    Q_PROPERTY(QString debug READ debug WRITE setDebug NOTIFY debugChanged)

public:
    explicit VoiceStore(ClientActions* clientActions,QObject *parent = nullptr);

    QString connectionAddress() const;
    void setConnectionAddress(const QString &newConnectionAddress);

    int connectionPort() const;
    void setConnectionPort(const int &newConnectionPort);

    QString connectionIp() const;
    void setConnectionIp(const QString &newConnectionIp);

    QString debug() const;
    void setDebug(const QString &newDebug);

signals:
    void connectionAddressChanged();
    void connectionPortChanged();

    void connectionIpChanged();

    void debugChanged();

private:
    void connectWebsocket();
    void disconnectWebsocket();

    void onConnected();
    void onDisconnected();
    void onMsgListened(QString message);

    bool parseMsg(QString message);
    int translateMsg(QString message);

    QWebSocket m_webSocket;
    QString m_connectionAddress;
    int m_connectionPort{8181};
    QString m_connectionIp{"192.168.1.111"};//"10.203.180.2"}; // chipsee ip

    ClientActions* m_clientActions;
    QString m_debug;
};

#endif // VOICE_STORE_H
