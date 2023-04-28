#include "voice_store.h"
#include <QQmlEngine>

VoiceStore::VoiceStore(ClientActions* clientActions,QObject *parent)
    : QObject(parent)
{
    m_clientActions= clientActions;
    setConnectionAddress("ws://"+connectionIp()+":" +QString(connectionPort())+"/core");
    connect(&m_webSocket, &QWebSocket::connected, this, &VoiceStore::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &VoiceStore::onDisconnected);
    connectWebsocket();
}

QUrl VoiceStore::connectionAddress() const
{
    return m_connectionAddress;
}

void VoiceStore::setConnectionAddress(const QUrl &newConnectionAddress)
{
    if (m_connectionAddress == newConnectionAddress)
        return;
    m_connectionAddress = newConnectionAddress;
    emit connectionAddressChanged();
}

int VoiceStore::connectionPort() const
{
    return m_connectionPort;
}

void VoiceStore::setConnectionPort(const int &newConnectionPort)
{
    if (m_connectionPort == newConnectionPort)
        return;
    m_connectionPort = newConnectionPort;
    emit connectionPortChanged();
}

void VoiceStore::connectWebsocket()
{
    m_webSocket.open(connectionAddress());

}

void VoiceStore::disconnectWebsocket()
{
    m_webSocket.close();
}

QString VoiceStore::connectionIp() const
{
    return m_connectionIp;
}

void VoiceStore::setConnectionIp(const QString &newConnectionIp)
{
    if (m_connectionIp == newConnectionIp)
        return;
    m_connectionIp = newConnectionIp;
    emit connectionIpChanged();
}

void VoiceStore::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &VoiceStore::onMsgListened);
    //m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void VoiceStore::onDisconnected()
{
    qDebug() << "WebSocket disconnected";
}

void VoiceStore::onMsgListened(QString message)
{
    qDebug() << "Message received:" << message;
    if(parseMsg(message)){
         qDebug() << "Performed action";
    }
}

bool VoiceStore::parseMsg(QString message)
{
    //HERE PARSE MSG FROM WEBSOCKET
    QVariant fullMessage = QVariant(message);
    QJsonDocument jsonResponse = QJsonDocument::fromVariant(fullMessage);

    uint value=0;//here parse from the json the actual value;
    int translatedMsg= translateMsg(message);

    switch(translatedMsg){
    case 0:

        break;
    case 1:
        m_clientActions->startBlower();
        return true;
        break;
    case 2:
        m_clientActions->stopBlower();
        return true;
        break;
    case 3:
        m_clientActions->writeNominalFrequency(value);
        return true;
        break;
    case 4:
        m_clientActions->increaseNominalFrequency(value);
        return true;
        break;
    case 5:
        m_clientActions->decreaseNominalFrequency(value);
        return true;
        break;
    default:
        return false;
        break;
    }

}

int VoiceStore::translateMsg(QString message)
{
    if(message=="start")
        return int(TypeMsg::Start);
    else if(message=="stop")
        return int(TypeMsg::Stop);
    else if(message=="set")
        return int(TypeMsg::Set);
    else if(message=="increase")
        return int(TypeMsg::Increase);
    else if(message=="decrease")
        return int(TypeMsg::Increase);
    else
        return int(TypeMsg::Unknown);

}

