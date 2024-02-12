import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import FPZBlowerMonitorConf 1.0
import "../FPZBlowerMonitorConfUI"

ReadCurrentInverterParamsTpl {

    id: readCurrentParamsView
    width: Constants.width
    height: Constants.height
    property bool sensorDataIsComing: false
    property string testDeviceName
    anchors.fill: parent
    statusBar.connectedDeviceName.text: inveorStore.deviceId
    state: voiceStore.restarting ? "restarting" : ((voiceStore.reachableMycroft && voiceStore.reachableStt) ? "init" : "inactive")
    visible: true
    actualFreqReader.value: inveorStore.actualFrequency
    motorVoltReader.value: inveorStore.motorVoltage
    motorCurrReader.value: inveorStore.motorCurrent
    alertMsg.message: inveorStore.msgStatus
    increaseNominalFreqBtn.onClicked: clientActions.increaseNominalFrequency(1)
    decreaseNominalFreqBtn.onClicked: clientActions.decreaseNominalFrequency(1)
    quitAppBtn.onClicked: clientActions.quitApp()
    connectionBtn.onCheckedChanged: connectionBtn.checked ? clientActions.connectDevice() : clientActions.disconnectDevice()
    connectionBtn.text: connectionBtn.checked ? "DISC." : "CONN."
    connectionBtn.display: AbstractButton.TextBesideIcon
    quitAppBtn.display: AbstractButton.TextBesideIcon
    increaseNominalFreqBtn.display: AbstractButton.TextBesideIcon
    decreaseNominalFreqBtn.display: AbstractButton.TextBesideIcon

    statusBar.modbusConnectionIndicator.state: inveorStore.connStatus === "Connected" ? "connected" : ""
    statusBar.language: voiceStore.language
    statusBar.voiceConnectionIndicator.state: (voiceStore.reachableMycroft && voiceStore.reachableStt) ? "connected" : "disabled"

    statusBar.changeLanguageBtn.onClicked: clientActions.switchLanguage()

    language: voiceStore.language
    activeCommand: voiceStore.activeCommand
    listenedCommand: voiceStore.listenedCommand
    commandName: voiceStore.commandName
    commandValue: voiceStore.commandValue


changeStateBtn.onClicked: {
    if(readCurrentParamsView.state=="init"){
        readCurrentParamsView.state="restarting"
    }else{
        readCurrentParamsView.state="init"
    }
    }

    manualListenBtn.onClicked: clientActions.triggerWakeWord()

}

