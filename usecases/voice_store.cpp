#include "voice_store.h"
#include <QQmlEngine>
#include <QDebug>

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
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &VoiceStore::onMsgListened);
    //connect(&m_webSocket, &QWebSocket::aboutToClose, this, &VoiceStore::onDisconnected);

    m_checkReceivedTimer = new QTimer(this);
    m_checkReceivedTimer->setInterval(5000);
    connect(m_checkReceivedTimer, &QTimer::timeout, this, [this]{
        setReceivedMsg(false);
    });

    m_checkConnectionTimer = new QTimer(this);
    m_checkConnectionTimer->setInterval(5000);

    connect(m_checkConnectionTimer, &QTimer::timeout, this, [this]{

        qDebug()<< "WEBSOCKET VALID: "<< websocketValid();

        if(websocketValid()==false && m_webSocket.state() == QAbstractSocket::UnconnectedState)
        {

            connectWebsocket();
            setReachableMycroft(false);
        }

        else {

            setRestarting(false);
            setReachableMycroft(true);
        }


        //        if(m_msgCounter==0)
        //        {
        //            setReachableMycroft(false);
        //        }
        //        else {
        //            setRestarting(false);
        //            setReachableMycroft(true);
        //            //setReachableStt(true);
        //        }



        //setMsgCounter(0);


        if(m_sttUnreachableCounter==0){
            setReachableStt(true);
        } else {
            QJsonArray testArr= {"test"};
            QJsonObject dataObject
            {

                {"utterances", testArr},
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

            m_sttUnreachableCounter=0;


            if(!reachableSkills()){
                onCheckSkillsLoad();
            }

        });



        m_checkConnectionTimer->start();

        connect(m_clientActions,&ClientActions::triggerWakeWord,this,&VoiceStore::onTriggeredWakeWord);
        connect(m_clientActions,&ClientActions::switchLanguage,this,&VoiceStore::onSwitchLanguage);
        connect(m_clientActions,&ClientActions::checkLanguage,this,&VoiceStore::onCheckLanguage);

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
        setWebsocketValid(true);

    }

    void VoiceStore::disconnectWebsocket()
    {
        m_webSocket.close();
    }
    //
    void VoiceStore::onTriggeredWakeWord()
    {
        setDetectedWakeWord(true);
        QJsonObject jsonObj;
        jsonObj.insert("type","mycroft.mic.listen");
        jsonObj.insert("data","true");

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
        m_connectingMsgCount = m_connectingMsgCount +1;
        setDebug("on connected " + QString::number(m_connectingMsgCount));
        qDebug() << "WebSocket connected";
        qDebug() << m_webSocket.state();

    }

    void VoiceStore::onDisconnected()
    {
        setWebsocketValid(false);
        qDebug() << "WebSocket disconnected";
        qDebug() << m_webSocket.state();
    }

    void VoiceStore::onMsgListened(QString message)
    {
        uint newMsgCounter= msgCounter()+1;
        setMsgCounter(newMsgCounter);
        qDebug() << m_webSocket.state();
        qDebug().noquote() << "Message received:" << message;
        if(parseMsg(message)){
            qDebug() << "Performed action";
        }
    }

    void VoiceStore::onSwitchLanguage()
    {

        QString inputLingua;

        QString lang;
        if(language()=="IT"){
            inputLingua="lingua inglese";
            lang="it-it";
        }else{
            inputLingua="language italian";
            lang="en-us";
        }
        QJsonValue linguaVal(inputLingua);
        QJsonArray linguaArray= {inputLingua};
        QJsonObject dataObject
        {
            {"utterances", linguaArray},
            {"lang", lang},
            {"session","0"}
        };

        qDebug() << dataObject;
        QJsonObject object
        {
            {"type", "recognizer_loop:utterance"},
            {"data", dataObject}
        };

        QJsonDocument doc(object);

        QByteArray jsonMessage(doc.toJson(QJsonDocument::Compact));
        QString stringMessage = QString::fromUtf8(jsonMessage);

        qDebug() << stringMessage;
        m_webSocket.sendTextMessage(stringMessage);

        setLanguage(language()=="IT" ? "EN" : "IT" );


        setRestarting(true);
        setReachableMycroft(false);
        setReachableSkills(false);


        disconnectWebsocket();

        m_checkConnectionTimer->start();
    }

    void VoiceStore::onCheckLanguage()
    {
        QString checkCommand= "Check current lang";

        QJsonArray checkLangArray= {checkCommand};

        QJsonObject dataObject
        {
            {"utterances", checkLangArray},
            {"lang", "it-it"},
            {"session","1"}
        };

        qDebug() << dataObject;
        QJsonObject object
        {
            {"type", "recognizer_loop:utterance"},
            {"data", dataObject}
        };

        QJsonDocument doc(object);

        QByteArray jsonMessage(doc.toJson(QJsonDocument::Compact));
        QString stringMessage = QString::fromUtf8(jsonMessage);

        qDebug() << stringMessage;
        m_webSocket.sendTextMessage(stringMessage);
    }

    void VoiceStore::onCheckSkillsLoad()
    {

        QJsonObject object
        {
            {"type", "skillmanager.list"}
        };

        QJsonDocument doc(object);

        QByteArray jsonMessage(doc.toJson(QJsonDocument::Compact));
        QString stringMessage = QString::fromUtf8(jsonMessage);

        //qDebug() << stringMessage;
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

        if(valuetype.toString()=="mycroft.skills.initialized"){
            setReachableSkills(true);
            onCheckLanguage();
        }

        if(valuetype.toString()=="mycroft.skills.fallback"){
            if(reachableSkills()){
            setListenedCommand(msg);
            setActiveCommand(false);
            setReceivedMsg(true);
            m_checkReceivedTimer->start();
            }
        }

        if(valuetype.toString()=="fpzcontrol"){
            if(translatedMsg !=7){
                setActiveCommand(true);
                setReceivedMsg(true);
                m_checkReceivedTimer->start();
            }
        }

        if(valuetype.toString()=="recognizer_loop:wakeword"){
            setDetectedWakeWord(true);
        }
        if(valuetype.toString()=="recognizer_loop:record_end"){
            setDetectedWakeWord(false);
        }

        if(valuetype.toString()=="enclosure.notify.no_internet"){
            setReachableStt(false);
            m_sttUnreachableCounter+=1;
        }


        if(valuetype.toString()=="mycroft.skills.list"){
            QJsonValue valueskill = item.value("fpz-voice-poc-skill");
            QJsonObject fpzskill= valueskill.toObject();

            bool valuefpzactive=fpzskill["active"].toBool();

            setReachableSkills(valuefpzactive);
            onCheckLanguage();
        }

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
            setRestarting(true);
            m_checkConnectionTimer->start();
            return true;

            case 7:
            if(value==int(Lang::Italian)){
                setLanguage("IT");
            }
            else if(value==int(Lang::English)){
                setLanguage("EN");
            }
            else{
                setLanguage("IT");
            }
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
        else if(message=="checklang")
        return int(TypeMsg::CheckLang);
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

    bool VoiceStore::reachableMycroft() const
    {
        return m_reachableMycroft;
    }

    void VoiceStore::setReachableMycroft(bool newReachable)
    {
        if (m_reachableMycroft == newReachable)
        return;
        m_reachableMycroft = newReachable;
        emit reachableMycroftChanged();
    }


    bool VoiceStore::restarting() const
    {
        return m_restarting;
    }

    void VoiceStore::setRestarting(bool newRestarting)
    {
        if (m_restarting == newRestarting)
        return;
        m_restarting = newRestarting;
        emit restartingChanged();
    }

    bool VoiceStore::reachableStt() const
    {
        return m_reachableStt;
    }

    void VoiceStore::setReachableStt(bool newReachableStt)
    {
        if (m_reachableStt == newReachableStt)
        return;
        m_reachableStt = newReachableStt;
        emit reachableSttChanged();
    }

    uint VoiceStore::msgCounter() const
    {
        return m_msgCounter;
    }

    void VoiceStore::setMsgCounter(uint newMsgCounter)
    {
        if (m_msgCounter == newMsgCounter)
        return;
        m_msgCounter = newMsgCounter;
        emit msgCounterChanged();
    }

    bool VoiceStore::websocketValid() const
    {
        return m_websocketValid;
    }

    void VoiceStore::setWebsocketValid(bool websocketValid)
    {
        if (m_websocketValid == websocketValid)
        return;
        m_websocketValid = websocketValid;
        emit websocketValidChanged();

    }

    bool VoiceStore::reachableSkills() const
    {
        return m_reachableSkills;
    }

    void VoiceStore::setReachableSkills(bool reachableSkills)
    {
        if (m_reachableSkills == reachableSkills)
        return;

        m_reachableSkills = reachableSkills;
        emit reachableSkillsChanged();
    }

    bool VoiceStore::receivedMsg() const
    {
        return m_receivedMsg;
    }

    void VoiceStore::setReceivedMsg(bool receivedMsg)
    {
        if (m_receivedMsg == receivedMsg)
        return;

        m_receivedMsg = receivedMsg;
        emit receivedMsgChanged();
    }
