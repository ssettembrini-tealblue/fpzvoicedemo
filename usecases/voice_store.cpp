#include "voice_store.h"
#include <QQmlEngine>

VoiceStore::VoiceStore(ClientActions* clientActions,QObject *parent)
    : QObject(parent)
{
    qDebug() << m_webSocket.state();
    m_clientActions= clientActions;
    emit m_clientActions->connectDevice();
    setDetectedWakeWord(false);

    setConnectionAddress("ws://"+connectionIp()+":" + QString::number(connectionPort()) +"/core");
    qDebug() << connectionAddress();

    connect(&m_webSocket, &QWebSocket::connected, this, &VoiceStore::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &VoiceStore::onDisconnected);


    m_checkConnectionTimer = new QTimer(this);
    m_checkConnectionTimer->setInterval(5000);

    connect(m_checkConnectionTimer, &QTimer::timeout, this, [this]{

        if(m_msgCounter==0)
        {
            setReachable(false);
        }
        else {
            setReachable(true);
        }

        QJsonObject jsonObj;
        jsonObj.insert("type","skillmanager.list");//"mycroft.audio.service.list_backends");
        jsonObj.insert("data","true");
        QJsonDocument doc(jsonObj);
        QByteArray jsonMessage(doc.toJson(QJsonDocument::Compact));
        QString stringMessage = QString::fromUtf8(jsonMessage);
        m_webSocket.sendTextMessage(stringMessage);

        m_msgCounter=0;

    });

    m_checkConnectionTimer->start();

    connect(m_clientActions,&ClientActions::triggerWakeWord,this,&VoiceStore::onTriggeredWakeWord);

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

void VoiceStore::onTriggeredWakeWord()
{
    QJsonObject jsonObj;
    jsonObj.insert("type","mycroft.mic.listen");
    jsonObj.insert("data","true");

    //    QJsonObject dataObject;
    //        dataObject.insert("Street", "Downing Street 10");
    //        dataObject.insert("City", "London");
    //        jsonObj.insert("data", dataObject);

    QJsonDocument doc(jsonObj);

    QByteArray jsonMessage(doc.toJson(QJsonDocument::Compact));
    QString stringMessage = QString::fromUtf8(jsonMessage);
    m_webSocket.sendTextMessage(stringMessage);
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
    m_msgCounter+=1;
    qDebug() << m_webSocket.state();
    qDebug().noquote() << "Message received:" << message;
    if(parseMsg(message)){
        qDebug() << "Performed action";
    }
}

void VoiceStore::sendLanguageChange()
{
//     recognizer_loop:utterance
//     STT has detected the given text or text was injected as an utterance via the CLI.
//     Data:
//      {
//        "utterances": [text],
//        "lang": self.stt.lang,
//        "session": session_id
//     }

    QJsonObject dataObject
        {
            {"utterances", language()=="IT" ? "lingua inglese" : "language italian"},
            {"lang", language()=="IT" ? "it-it" : "en-us"},
            {"session","0"}
        };


    QJsonObject object
        {
            {"type", "recognizer_loop:utterance"},
            {"data", dataObject}
        };

    QJsonDocument doc(object);

    QByteArray jsonMessage(doc.toJson(QJsonDocument::Compact));
    QString stringMessage = QString::fromUtf8(jsonMessage);
    m_webSocket.sendTextMessage(stringMessage);
}

bool VoiceStore::parseMsg(QString message)
{

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());//fromVariant(fullMessage);
    QJsonObject json = doc.object();

    QJsonValue valuetype = json.value("type");
    QJsonValue valuedata = json.value("data");

    QJsonObject item = valuedata.toObject();

    uint value=item["value"].toInt(); //here parse from the json the actual value;
    QString msg=item["utterance"].toString();

    int translatedMsg=translateMsg(item["action"].toString());

    if(valuetype.toString()=="mycroft.skills.fallback"){
        setListenedCommand(msg);
        setActiveCommand(false);
    }

    if(valuetype.toString()=="fpzcontrol"){

        setActiveCommand(true);
    }

    if(valuetype.toString()=="recognizer_loop:wakeword"){
        setDetectedWakeWord(true);
    }
    if(valuetype.toString()=="recognizer_loop:record_end"){
        setDetectedWakeWord(false);
    }

    setDebug(message.toUtf8());

    switch(translatedMsg){
    case 0:
        qDebug() << "unknown cmd" << "\n";
        return false;
    case 1:
        emit m_clientActions->startBlower();
        setCommandName("StartBlower");
        setCommandValue(0);
        qDebug() << "Started blower" << "\n";

        return true;
    case 2:
        emit m_clientActions->stopBlower();
        setCommandName("StopBlower");
        setCommandValue(0);
        qDebug() << "Stopped blower" << "\n";
        return true;
    case 3:
        //if(value<40){
        emit m_clientActions->writeNominalFrequency(value);
        setCommandName("SetFrequency");
        setCommandValue(value);
        qDebug() << "Written nominal frequency" << "\n";
        //}
        return true;
    case 4:
        emit m_clientActions->increaseNominalFrequency(value);
        setCommandName("IncreaseFrequency");
        setCommandValue(value);
        qDebug() << "Increased nominal frequency" << "\n";
        return true;
    case 5:
        emit m_clientActions->decreaseNominalFrequency(value);
        setCommandName("DecreaseFrequency");
        setCommandValue(value);
        qDebug() << "Decreased nominal frequency" << "\n";
        return true;
    case 6:
        if(value==0)
            setLanguage("IT");
        else if(value==1)
            setLanguage("EN");
        qDebug() << "Switch language" << "\n";
        return true;
    default:
        qDebug() << "default" << "\n";
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
        return int(TypeMsg::Decrease);
    else if(message=="switch")
        return int(TypeMsg::Switch);
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

bool VoiceStore::detectedWakeWord() const
{
    return m_detectedWakeWord;
}

void VoiceStore::setDetectedWakeWord(bool newDetectedWakeWord)
{
    if (m_detectedWakeWord == newDetectedWakeWord)
        return;
    m_detectedWakeWord = newDetectedWakeWord;
    emit detectedWakeWordChanged();
}

QString VoiceStore::listenedCommand() const
{
    return m_listenedCommand;
}

void VoiceStore::setListenedCommand(const QString &newListenedCommand)
{
    if (m_listenedCommand == newListenedCommand)
        return;
    m_listenedCommand = newListenedCommand;
    emit listenedCommandChanged();
}

bool VoiceStore::activeCommand() const
{
    return m_activeCommand;
}

void VoiceStore::setActiveCommand(const bool &newActiveCommand)
{
    if (m_activeCommand == newActiveCommand)
        return;
    m_activeCommand = newActiveCommand;
    emit activeCommandChanged();
}

QString VoiceStore::language() const
{
    return m_language;
}

void VoiceStore::setLanguage(const QString &newLanguage)
{
    if (m_language == newLanguage)
        return;
    m_language = newLanguage;
    emit languageChanged();
}

QString VoiceStore::commandName() const
{
    return m_commandName;
}

void VoiceStore::setCommandName(const QString &newCommandName)
{
    if (m_commandName == newCommandName)
        return;
    m_commandName = newCommandName;
    emit commandNameChanged();
}

uint VoiceStore::commandValue() const
{
    return m_commandValue;
}

void VoiceStore::setCommandValue(const uint &newCommandValue)
{
    if (m_commandValue == newCommandValue)
        return;
    m_commandValue = newCommandValue;
    emit commandValueChanged();
}

bool VoiceStore::reachable() const
{
    return m_reachable;
}

void VoiceStore::setReachable(bool newReachable)
{
    if (m_reachable == newReachable)
        return;
    m_reachable = newReachable;
    emit reachableChanged();
}
