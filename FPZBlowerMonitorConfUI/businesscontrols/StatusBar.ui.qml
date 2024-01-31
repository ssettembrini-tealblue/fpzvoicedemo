import QtQuick 2.15
import FPZBlowerMonitorConf 1.0
import QtGraphicalEffects 1.12
import "themes"

Item {
    id: statusArea
    readonly property alias connectedDeviceName: connectedDeviceName
    readonly property alias sensorDataUpdateStatus: sensorDataUpdateStatus
    readonly property alias btConnectionIndicator: btConnectionIndicator
    readonly property alias modbusConnectionIndicator: modbusConnectionIndicator
    property alias languageIndicator: languageIndicator
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
            //anchors.left: parent.left
            //anchors.leftMargin: 16
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


    Image {
        id: image
        anchors.right: parent.right
        anchors.rightMargin: 24
        visible: true
        source: languageIndicator.text=="IT" ? "icons/it.svg" : "icons/gb.svg"
        height: 36
        width: 42
        anchors.verticalCenter: parent.verticalCenter
        sourceSize.height: height
        sourceSize.width: width
        fillMode: Image.PreserveAspectFit
        layer.enabled: true
        layer.effect: OpacityMask{

            maskSource: Item{
                height:image.height
                width:image.width
                Rectangle {
                    anchors.centerIn: parent

                    //color: "white"
                    opacity: 0.85
                    height: 36
                    width: 36
                    radius: 18
                }
            }

        }}
    Text {
        id: languageIndicator
        visible: false
        anchors.centerIn: parent
        //text: "IT"
        font.family: Constants.fontMainS.family
        font.weight: Constants.fontMainS.weight
        font.pixelSize: 22 //Constants.fontMainM.pixelSize
        color: "black"
    }

}

/*##^##
Designer {
    D{i:0;height:64;width:1024}
}
##^##*/

