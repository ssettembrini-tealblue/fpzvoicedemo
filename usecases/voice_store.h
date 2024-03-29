#ifndef VOICE_STORE_H
#define VOICE_STORE_H

#include <QObject>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QWebSocket>
#include "client_actions.h"
#include <QTimer>

enum class TypeMsg
{
    Start = 1,
    Stop = 2,
    Set = 3,
    Increase = 4,
    Decrease = 5,
    Switch = 6,
    CheckLang = 7,
    Unknown = 0
};

enum class Lang{
    Italian = 1,
    English = 2,
    Unknown = 0

};


class VoiceStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString connectionIp READ connectionIp WRITE setConnectionIp NOTIFY connectionIpChanged)
    Q_PROPERTY(int connectionPort READ connectionPort WRITE setConnectionPort NOTIFY connectionPortChanged)
    Q_PROPERTY(QString connectionAddress READ connectionAddress WRITE setConnectionAddress NOTIFY connectionAddressChanged)
    Q_PROPERTY(QString debug READ debug WRITE setDebug NOTIFY debugChanged)
    Q_PROPERTY(bool detectedWakeWord READ detectedWakeWord WRITE setDetectedWakeWord NOTIFY detectedWakeWordChanged)
    Q_PROPERTY(QString listenedCommand READ listenedCommand WRITE setListenedCommand NOTIFY listenedCommandChanged)
    Q_PROPERTY(bool activeCommand READ activeCommand WRITE setActiveCommand NOTIFY activeCommandChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString commandName READ commandName WRITE setCommandName NOTIFY commandNameChanged)
    Q_PROPERTY(uint commandValue READ commandValue WRITE setCommandValue NOTIFY commandValueChanged)
    Q_PROPERTY(uint msgCounter READ msgCounter WRITE setMsgCounter NOTIFY msgCounterChanged)

    Q_PROPERTY(bool receivedMsg READ receivedMsg WRITE setReceivedMsg NOTIFY receivedMsgChanged)

    Q_PROPERTY(bool websocketValid READ websocketValid WRITE setWebsocketValid NOTIFY websocketValidChanged)
    Q_PROPERTY(bool reachableMycroft READ reachableMycroft WRITE setReachableMycroft NOTIFY reachableMycroftChanged FINAL)
    Q_PROPERTY(bool reachableStt READ reachableStt WRITE setReachableStt NOTIFY reachableSttChanged FINAL)
    Q_PROPERTY(bool reachableSkills READ reachableSkills WRITE setReachableSkills NOTIFY reachableSkillsChanged FINAL)
    Q_PROPERTY(bool restarting READ restarting WRITE setRestarting NOTIFY restartingChanged FINAL)
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

    bool detectedWakeWord() const;
    void setDetectedWakeWord(bool newDetectedWakeWord);

    QString listenedCommand() const;
    void setListenedCommand(const QString &newListenedCommand);

    bool activeCommand() const;
    void setActiveCommand(const bool &newActiveCommand);

    QString language() const;
    void setLanguage(const QString &newLanguage);

    QString commandName() const;
    void setCommandName(const QString &newCommandName);

    uint commandValue() const;
    void setCommandValue(const uint &newCommandValue);

    bool reachableMycroft() const;
    void setReachableMycroft(bool newReachable);

    bool restarting() const;
    void setRestarting(bool newRestarting);

    bool reachableStt() const;
    void setReachableStt(bool newReachableStt);

    uint msgCounter() const;
    void setMsgCounter(uint newMsgCounter);

    bool websocketValid() const;
    void setWebsocketValid(bool websocketValid);

    bool reachableSkills() const;


    void setReachableSkills(bool reachableSkills);

    bool receivedMsg() const;

    void setReceivedMsg(bool receivedMsg);

signals:
    void connectionAddressChanged();
    void connectionPortChanged();

    void connectionIpChanged();

    void debugChanged();

    void detectedWakeWordChanged();

    void listenedCommandChanged();

    void activeCommandChanged();

    void languageChanged();

    void commandNameChanged();

    void commandValueChanged();

    void restartingChanged();

    void reachableSttChanged();

    void reachableMycroftChanged();

    void msgCounterChanged();

    void websocketValidChanged();

    void reachableSkillsChanged();

    void receivedMsgChanged();

private:
    void connectWebsocket();
    void disconnectWebsocket();

    void onTriggeredWakeWord();
    void onConnected();
    void onDisconnected();
    void onMsgListened(QString message);


    void onSwitchLanguage();

    void onCheckLanguage();
    void onCheckSkillsLoad();

    bool parseMsg(QString message);
    int translateMsg(QString message);

    QWebSocket m_webSocket;
    QString m_connectionAddress;
    int m_connectionPort{8181};
    QString m_connectionIp{"127.0.0.1"};//{"192.168.1.111"};//"10.203.180.2"}; // chipsee ip

    ClientActions* m_clientActions;
    QString m_debug;
    bool m_detectedWakeWord {false};
    QString m_listenedCommand {""};
    bool m_activeCommand{false};
    QString m_language{"IT"};
    QString m_commandName;
    uint m_commandValue;

    QTimer* m_checkConnectionTimer;
    QTimer* m_checkSttTimer;
    QTimer* m_checkReceivedTimer;

    uint m_msgCounter {0};
    uint m_sttUnreachableCounter {0};
    bool m_restarting {false};
    bool m_reachableStt{false};
    bool m_reachableMycroft{false};
    bool m_websocketValid{false};
    bool m_reachableSkills{false};
    bool m_receivedMsg{false};
    uint m_connectingMsgCount{0};
};

#endif // VOICE_STORE_H
