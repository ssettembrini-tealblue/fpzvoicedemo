import QtQuick 2.15
import QtQuick.Controls 2.15
import FPZBlowerMonitorConf 1.0

import "themes"

Item {
    id: statusArea
    readonly property alias connectedDeviceName: connectedDeviceName
    readonly property alias sensorDataUpdateStatus: sensorDataUpdateStatus
    readonly property alias btConnectionIndicator: btConnectionIndicator
    readonly property alias modbusConnectionIndicator: modbusConnectionIndicator
    readonly property alias voiceConnectionIndicator: voiceConnectionIndicator
   // readonly property alias infoBtn: infoBtn

    implicitWidth: 640
    implicitHeight: 64
    StatusBarBg {
        id: statusAreaBg
        anchors.fill: parent
    }
    Row{
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.verticalCenter: parent.verticalCenter
        spacing: 16
        ModbusConnectionIndicator {
            id: modbusConnectionIndicator
            anchors.verticalCenter: parent.verticalCenter
            //anchors.right: parent.right
            //anchors.rightMargin: 16
        }
        SensorDataUpdateStatus {
            id: sensorDataUpdateStatus
            secs.visible: false
            sensorDataLastUpdateLabel.visible: false
            //anchors.left: parent.left
            //anchors.leftMargin: 16
            anchors.verticalCenter: parent.verticalCenter
        }
        VoiceConnectionIndicator{
            id: voiceConnectionIndicator
            visible:false
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Text {
        id: connectedDeviceName
        color: ThemeConstants.colorStatusFg
        text: qsTr("<connected device name>")
        anchors.verticalCenter: parent.verticalCenter
        font: Constants.fontMainM
        anchors.horizontalCenter: parent.horizontalCenter
    }

    BtConnectionIndicator {
        id: btConnectionIndicator
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 16
    }
    // IconBtn{
    //     id: infoBtn
    //     icon.name: "info-circle"
    //     height: 48
    //     width: 48
    //     color: ThemeConstants.colorInfo
    //     anchors.verticalCenter: parent.verticalCenter
    //     anchors.right: parent.right
    //     anchors.rightMargin: 16
    // }




}

/*##^##
Designer {
    D{i:0;height:64;width:1024}
}
##^##*/

