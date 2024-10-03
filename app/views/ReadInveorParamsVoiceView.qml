import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import FPZBlowerMonitorConf 1.0
import "../FPZBlowerMonitorConfUI"

ReadCurrentInverterParamsVoiceTpl {

    id: readCurrentParamsView
    width: Constants.width
    height: Constants.height
    property bool sensorDataIsComing: false
    property string testDeviceName
    anchors.fill: parent
    statusBar.connectedDeviceName.text: ""//inveorStore.deviceId
    state: voiceStore.restarting ? "restarting" : ((voiceStore.reachableMycroft && voiceStore.reachableStt) ? (voiceStore.reachableSkills ? "init" : "inactiveskill") : "inactive")
    visible: true
    actualFreqReader.value: inveorStore.actualFrequency
    motorVoltReader.value: inveorStore.motorVoltage
    motorCurrReader.value: inveorStore.motorCurrent
    alertMsg.message: inveorStore.msgStatus
    increaseNominalFreqBtn.onClicked: clientActions.increaseNominalFrequency(1)
    decreaseNominalFreqBtn.onClicked: clientActions.decreaseNominalFrequency(1)
    quitAppBtn.onClicked: clientActions.quitApp()
    toggleDO2Btn.onClicked: inveorStore.digOut2 ? clientActions.stopDO2() : clientActions.startDO2()
    connectionBtn.onCheckedChanged: connectionBtn.checked ? clientActions.connectDevice() : clientActions.disconnectDevice()
    connectionBtn.text: connectionBtn.checked ? "DISC." : "CONN."
    connectionBtn.display: AbstractButton.TextBesideIcon
    quitAppBtn.display: AbstractButton.TextBesideIcon
    increaseNominalFreqBtn.display: AbstractButton.TextBesideIcon
    decreaseNominalFreqBtn.display: AbstractButton.TextBesideIcon
    //statusBar.
    infoBtn.onClicked:{
        infoOverlayRegion.open()
    }
    infoOverlay.lang: voiceStore.language
    statusBar.modbusConnectionIndicator.state: inveorStore.connStatus === "Connected" ? "connected" : ""
    statusBar.voiceConnectionIndicator.state: (voiceStore.reachableMycroft && voiceStore.reachableStt) ? "connected" : "disabled"
    statusBar.wakewordDetectedIndicator.enabled: (voiceStore.reachableMycroft && voiceStore.reachableStt)
    //manualDetectionTimer.onTriggered: //detectedWakeWord=undefined
    manualListenBtn.onClicked: clientActions.triggerWakeWord()
    changeLanguageBtn.onClicked: clientActions.switchLanguage()
    debug: voiceStore.debug
    language: voiceStore.language
    commandName: voiceStore.commandName
    commandValue: voiceStore.commandValue
    detectedWakeWord: voiceStore.detectedWakeWord
    activeCommand: voiceStore.activeCommand
    listenedCommand: voiceStore.listenedCommand
    visibleBox: voiceStore.receivedMsg
    checkLanguageBtn.onClicked: clientActions.checkLanguage()


   // pidDebug.text: inveorStore.pidActualValue
    startDO2Btn.onClicked: {
        //pidDebug.text= "PID clicked"
        //clientActions.writePidActualValue(0)
        clientActions.startDO2()
    }
    stopDO2Btn.onClicked: {
        //pidDebug.text= "PID clicked"
        //clientActions.writePidActualValue(3)
        clientActions.stopDO2()
    }

    //Testing on cmd injection
    inject.onClicked:{
        listenedCommand= "changed"
    }
    //Testing on manual state change
    changeStateBtn.onClicked: {
        if(readCurrentParamsView.state=="init"){
            readCurrentParamsView.state="restarting"
        }else{
            readCurrentParamsView.state="init"
        }
    }


    Connections{
        target: inveorStore
        function onPidActualValueChanged(value){
            //pidDebug.text= "PID ACTUAL VALUE CHANGED TO " + value
        }
        // function onPidNominalValueChanged(value){
        //     pidDebug.text= "PID NOMINAL VALUE CHANGED TO" + value
        // }
    }

}

