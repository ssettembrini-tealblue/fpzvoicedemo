import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import FPZBlowerMonitorConf 1.0
import "../FPZBlowerMonitorConfUI"

ReadCurrentInverterParamsTpl {
    Row{
        spacing: 6
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 80
        Rectangle{
            height: 10
            width: 10
            radius: height/2
            color:voiceStore.detectedWakeWord ? "green" : "red"
            anchors.verticalCenter: parent.verticalCenter
        }

        Text{
            id: txtWakeWord
            text: voiceStore.detectedWakeWord ? "DETECTED" : "NOT DETECTED"
            color: voiceStore.detectedWakeWord ? "green" : "red"
            anchors.verticalCenter: parent.verticalCenter
        }
        Text{
            text: "-"
            visible: txtDebug.visible
            anchors.verticalCenter: parent.verticalCenter
        }
        Text{
            id: txtDebug
            visible: false
            text: voiceStore.debug
            color: voiceStore.detectedWakeWord ? "green" : "red"
            anchors.verticalCenter: parent.verticalCenter
        }
Item{
 width:30
 height:1
}
        Button{
            text: "Manual Listen"
            anchors.verticalCenter: parent.verticalCenter
            onClicked: clientActions.triggerWakeWord()
        }
    }
    id: readCurrentParamsView
    width: Constants.width
    height: Constants.height
    property bool sensorDataIsComing: false
    property string testDeviceName
    anchors.fill: parent
    statusBar.connectedDeviceName.text: inveorStore.deviceId
    state: "init"
    visible: true
    actualFreqReader.value: inveorStore.nomFreq//actualFrequency
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



}
