import QtQuick 2.15
import QtQuick.Controls 2.15
import FPZBlowerMonitorConf 1.0
import QtGraphicalEffects 1.12
import "themes"

Item {
    id: statusArea
    readonly property alias connectedDeviceName: connectedDeviceName
    readonly property alias sensorDataUpdateStatus: sensorDataUpdateStatus
    readonly property alias btConnectionIndicator: btConnectionIndicator
    readonly property alias modbusConnectionIndicator: modbusConnectionIndicator
    readonly property alias voiceConnectionIndicator: voiceConnectionIndicator
    property alias languageIndicator: languageIndicator
    property alias changeLanguageBtn: changeLanguageBtn
    property string language
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

    Button{
        id: changeLanguageBtn
        anchors.right: parent.right
        anchors.rightMargin: 24
        anchors.verticalCenter: parent.verticalCenter
        width: 74//rowLanguage.childrenRect.width + 40//72

        background: ActionBtnBg{
            width: parent.width //72
            height: 52
            down: changeLanguageBtn.pressed
        }
        contentItem:
            // Item{
            // anchors.verticalCenter: parent.verticalCenter
            // anchors.horizontalCenter: parent.horizontalCenter
            Row{
                id: rowLanguage
                spacing: 2
                //x:8
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                Item{
                    visible: false
                    height: 1
                    width: 4
                }

                Image {
                    id: image
                    //anchors.centerIn: parent
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: changeLanguageBtn.pressed ? 2 : 0
                    visible: true
                    source: language=="IT" ? "icons/gb.svg" : "icons/it.svg"
                    height: 36
                    width: 42

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
                    }
                }
                Item{
                    visible: false
                    height: 1
                    width: 4
                }
                Text {
                    id: languageIndicator
                    visible: false
                    //visible: false
                    anchors.verticalCenter: parent.verticalCenter
                    //anchors.centerIn: parent
                    text: language=="IT" ? "CAMBIA LINGUA" : "CHANGE LANGUAGE"
                    font.family: Constants.fontMainS.family
                    font.weight: Constants.fontMainS.weight
                    font.pixelSize: 20 //Constants.fontMainM.pixelSize
                    color: "black"
                }
          //  }
        }}


}

/*##^##
Designer {
    D{i:0;height:64;width:1024}
}
##^##*/

