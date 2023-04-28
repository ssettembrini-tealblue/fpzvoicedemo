#ifndef VOICE_STORE_H
#define VOICE_STORE_H

#include <QObject>
#include <QTimer>
#include <QJsonDocument>
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
    Q_PROPERTY(QUrl connectionAddress READ connectionAddress WRITE setConnectionAddress NOTIFY connectionAddressChanged)

public:
    explicit VoiceStore(ClientActions* clientActions,QObject *parent = nullptr);

    QUrl connectionAddress() const;
    void setConnectionAddress(const QUrl &newConnectionAddress);

    int connectionPort() const;
    void setConnectionPort(const int &newConnectionPort);

    QString connectionIp() const;
    void setConnectionIp(const QString &newConnectionIp);

signals:
    void connectionAddressChanged();
    void connectionPortChanged();

    void connectionIpChanged();

private:
    void connectWebsocket();
    void disconnectWebsocket();

    void onConnected();
    void onDisconnected();
    void onMsgListened(QString message);

    bool parseMsg(QString message);
    int translateMsg(QString message);

    QWebSocket m_webSocket;
    QUrl m_connectionAddress;
    int m_connectionPort{8181};
    QString m_connectionIp{"192.168.188.36"};

    ClientActions* m_clientActions;
};

#endif // VOICE_STORE_H
