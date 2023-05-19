#include "voice_store.h"
#include <QQmlEngine>

VoiceStore::VoiceStore(ClientActions* clientActions,QObject *parent)
    : QObject(parent)
{
    qDebug() << m_webSocket.state();
    m_clientActions= clientActions;


    setConnectionAddress("ws://"+connectionIp()+":" + QString::number(connectionPort()) +"/core");
    qDebug() << connectionAddress();




    connect(&m_webSocket, &QWebSocket::connected, this, &VoiceStore::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &VoiceStore::onDisconnected);

    connectWebsocket();


}

QString VoiceStore::connectionAddress() const
{
    return m_connectionAddress;
}

void VoiceStore::setConnectionAddress(const QString &newConnectionAddress)
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
    QUrl url(connectionAddress());
    m_webSocket.open(url);

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
    qDebug() << m_webSocket.state();
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &VoiceStore::onMsgListened);
    //m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void VoiceStore::onDisconnected()
{
    qDebug() << "WebSocket disconnected";
    qDebug() << m_webSocket.state();
}

void VoiceStore::onMsgListened(QString message)
{
    qDebug() << m_webSocket.state();
    qDebug().noquote() << "Message received:" << message;
    if(parseMsg(message)){
        qDebug() << "Performed action";
    }
}

bool VoiceStore::parseMsg(QString message)
{
    qDebug () << Q_FUNC_INFO << "\n";
    //HERE PARSE MSG FROM WEBSOCKET
    //QVariant fullMessage = QVariant(message);
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());//fromVariant(fullMessage);
    QJsonObject json = doc.object();

    QJsonValue valuetype = json.value(QString("type"));
    QJsonValue typeobj = valuetype["type"];
    qDebug().noquote() << typeobj.toString();
    if(typeobj.toString()!="fpzcontrol") return false;
    QJsonValue valuedata = json.value(QString("data"));
    //qDebug().noquote() << valuedata;

    QJsonObject item = valuedata.toObject();
    //qDebug().noquote() << tr("value: ") << item["value"].toString();
    qDebug().noquote() << tr("action: ") << item["action"].toString();


    qDebug().noquote() << json["type"].toString();


    uint value=item["value"].toInt();//0;//here parse from the json the actual value;
    qDebug() << value;

    int translatedMsg=translateMsg(item["action"].toString());

    setDebug(message.toUtf8());

    switch(translatedMsg){
    case 0:
        qDebug() << "unknown cmd" << "\n";
        return false;
    case 1:
        emit m_clientActions->startBlower();
        qDebug() << "Started blower" << "\n";

        return true;
    case 2:
        emit m_clientActions->stopBlower();
        qDebug() << "Stopped blower" << "\n";
        return true;
    case 3:
        emit m_clientActions->writeNominalFrequency(value);
        qDebug() << "Written nominal frequency" << "\n";
        return true;
    case 4:
        emit m_clientActions->increaseNominalFrequency(value);
        qDebug() << "Increased nominal frequency" << "\n";
        return true;
    case 5:
        emit m_clientActions->decreaseNominalFrequency(value);
        qDebug() << "Decreased nominal frequency" << "\n";
        return true;
    default:
        return false;
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


QString VoiceStore::debug() const
{
    return m_debug;
}

void VoiceStore::setDebug(const QString &newDebug)
{
    if (m_debug == newDebug)
        return;
    m_debug = newDebug;
    emit debugChanged();
}
