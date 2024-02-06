import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import FPZBlowerMonitorConf 1.0
import "./businesscontrols"
import "./businesscontrols/themes"


Item {
    id: root
    width: Constants.width
    height: Constants.contentHeight
    property alias quitAppBtn: quitAppBtn
    property alias connectionBtn: connectionBtn
    property alias statusBar: statusBar
    property alias actionArea: actionArea
    property alias alertMsg: alertMsg
    property alias actualFreqReader: actualFreqReader
    property alias motorVoltReader: motorVoltReader
    property alias motorCurrReader: motorCurrReader
    property alias decreaseNominalFreqBtn: decreaseNominalFreqBtn
    property alias increaseNominalFreqBtn: increaseNominalFreqBtn
    property alias actionRow: actionRow
    property alias manualListenBtn: manualListenBtn
    property alias changeStateBtn: changeStateBtn
    property bool activeCommand
    property string listenedCommand
    property string commandName
    property int commandValue
    property string language



    TemplateBg {
        id: templateBg
        anchors.fill: parent
    }
    //     Rectangle{

    //         color:"red"
    //         anchors.fill: parent
    //         opacity:0.5
    // }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        StatusBar {
            id: statusBar
            Layout.fillWidth: true
            btConnectionIndicator.visible: false
            modbusConnectionIndicator.visible: true
            sensorDataUpdateStatus.visible: true

        }

        // Item {

        //     Layout.preferredHeight: 96
        //     Layout.fillWidth: true
        //     Layout.alignment: Qt.AlignHCenter //| Qt.AlignVCenter
        Rectangle{
            id: bgEffect
            color:"transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true
            ColumnLayout {
                id: columnLayoutContent
                anchors.fill: parent

                Item{
                    Layout.fillHeight: true
                    width:1
                }
                Row{
                    spacing: 10
                    //anchors.horizontalCenter: parent.horizontalCenter
                    //anchors.top: parent.top
                    //anchors.topMargin: 80
                    Layout.alignment: Qt.AlignHCenter
                    Rectangle{
                        id: detectionBox
                        anchors.verticalCenter: parent.verticalCenter
                        width: 180
                        height: 40
                        radius: 12

                        color: enabled ? (voiceStore.detectedWakeWord ? "#a0db8e" : "#ff6f69") : "#cccccc"
                        opacity: enabled ? 1 :0.8
                        Row{
                            anchors.centerIn: parent
                            // Rectangle{
                            //     height: 10
                            //     width: 10
                            //     radius: height/2
                            //     color:voiceStore.detectedWakeWord ? "green" : "red"
                            //     anchors.verticalCenter: parent.verticalCenter
                            // }

                            Text{
                                id: txtWakeWord
                                text: voiceStore.detectedWakeWord ? "DETECTED" : "NOT DETECTED"
                                color: "black"//voiceStore.detectedWakeWord ? "green" : "red"
                                anchors.verticalCenter: parent.verticalCenter
                                //font.pixelSize: 24
                                //lineHeight: 1.2
                                opacity: 0.5
                                font: Constants.fontMainS
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
                        }
                    }
                    // Item{
                    //  width:30
                    //  height:1
                    // }
                    // Button{
                    //     //id: manualListenBtn old

                    //     height: 32
                    //     anchors.verticalCenter: parent.verticalCenter
                    //     //onClicked: clientActions.triggerWakeWord()

                    //     background: Rectangle {
                    //         height: 32
                    //         radius: 4
                    //         color: manualListenBtn.pressed ? "#cccccc" : "white"
                    //         border.color: "grey"
                    //         border.width: 1

                    //     }
                    //     contentItem: Row{
                    //         spacing:8
                    //         anchors.centerIn: parent
                    //         Text{

                    //             text: " Manual Listen"
                    //             font.pixelSize: 14
                    //             anchors.verticalCenter: parent.verticalCenter
                    //         }
                    //         Rectangle{
                    //             height: 10
                    //             width: 10
                    //             radius: height/2
                    //             color:"red"
                    //             anchors.verticalCenter: parent.verticalCenter
                    //         }
                    //     }
                    // }
                    Button  {
                        id: manualListenBtn

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: enabled ? -2 : 0
                        height: 32
                        width: 160
                        leftPadding: 15
                        rightPadding: 15
                        display: icon.source==="" ? AbstractButton.TextBesideIcon : AbstractButton.TextOnly
                        //icon.source: "icons/power_settings_new-24px.svg"
                        background: ActionBtnBg {
                            down: manualListenBtn.pressed
                            height:32
                        }
                        contentItem: Row{
                            opacity: enabled ? 1 : 0.5
                            spacing:8
                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: manualListenBtn.pressed ? 2 : 0
                            Text{

                                text: "Manual Listen"
                                font.capitalization: Font.AllUppercase
                                font.family: Constants.fontMain.family
                                font.weight: Constants.fontMain.weight
                                font.pixelSize:  13
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            Rectangle{
                                height: 10
                                width: 10
                                radius: height/2
                                color: enabled ? "red" : "grey"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                    }
                    ActionBtn {
                        id: changeStateBtn
                        visible: false
                        //nchors.verticalCenter: parent.verticalCenter
                        //anchors.right: parent.right
                        text: "Change"
                        //Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        icon.source: "businesscontrols/icons/add_black_24dp.svg"
                    }
                }


                Item{
                    Layout.preferredHeight: 16
                    width:1
                }

                Rectangle {
                    id: msgArea

                    width: 600
                    Layout.preferredWidth: currentParametersGrid.implicitWidth
                    radius:10//16
                    // height: 64
                    color:  ThemeConstants.colorInfo //"#cccccc"//

                    //border.color: "#5b5b5b"
                    //border.width: 1.5

                    Layout.preferredHeight: 72
                    //Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter //| Qt.AlignVCenter
                    //visible:false
                    Column{
                        anchors.centerIn: parent
                        Text {
                            id: voiceMsgLabel
                            //visible: !root.activeCommand
                            color:"#F5F4F4"//"#5b5b5b"//ThemeConstants.colorMsgFg
                            text: root.activeCommand ? "Comando riconosciuto" : "Comando non riconosciuto"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            //Layout.alignment: Qt.AlignHCenter
                            //anchors.fill: parent
                            anchors.horizontalCenter: parent.horizontalCenter
                            //lineHeight: 1.2
                            opacity: 0.9
                            font: Constants.fontMain
                        }
                        Text {
                            id: voiceMsgValue
                            //visible: !root.activeCommand
                            color:"#F5F4F4"//"#5b5b5b"//ThemeConstants.colorMsgFg
                            text:  root.activeCommand ? root.commandName + (root.commandValue != 0 ? " - " + root.commandValue.toString() : "") : "\"" + root.listenedCommand + "\""
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            //Layout.alignment: Qt.AlignHCenter
                            //anchors.fill: parent
                            anchors.horizontalCenter: parent.horizontalCenter
                            //lineHeight: 1.2
                            opacity: 0.9
                            font: Constants.fontMain
                        }
                    }
                    AlertMsg {
                        id: alertMsg
                        visible:false
                        y: 56
                        width: 572
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Row {
                        id: progressMsgArea
                        visible:false
                        width: 240
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        Text {
                            id: progressMsg
                            color: ThemeConstants.colorMsgFg
                            text: qsTr("")
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font: Constants.fontMain
                            anchors.topMargin: 0
                            anchors.bottomMargin: 0
                        }
                    }
                }


                Item{
                    Layout.preferredHeight: 16
                    width:1
                }
                Item{
                    id: monitoringPage
                    Layout.preferredHeight: 180
                    Layout.preferredWidth: 600
                    Layout.alignment: Qt.AlignHCenter
                    GridLayout {
                        id: currentParametersGrid
                        anchors.verticalCenter: parent.verticalCenter
                        columnSpacing: 16
                        //rowSpacing: 16
                        anchors.horizontalCenter: parent.horizontalCenter
                        rows: 1
                        columns: 4

                        ParamReaderCircle {
                            id: actualFreqReader
                            minimumValue: 0
                            maximumValue: 50
                            minWarningValue: 40
                            minErrorValue: 45
                            paramLabel.text: "Frequency"
                            unitLabel.text: "Hz"
                            paramIcon.source: "businesscontrols/icons/loop_black_24dp.svg"
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }

                        ParamReader {
                            id: motorVoltReader

                            minWarningValue: 750
                            minErrorValue: 900
                            paramIcon.source: "businesscontrols/icons/power_black_24dp.svg"
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            paramLabel.text: "Voltage"
                            unitLabel.text: "V"
                        }

                        ParamReader {
                            id: motorCurrReader

                            minWarningValue: 750
                            minErrorValue: 900
                            paramIcon.source: "businesscontrols/icons/bolt_black_24dp.svg"
                            paramLabel.text: "Power"
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            unitLabel.text: "A"
                        }

                    }
                }
                Item{
                    Layout.fillHeight: true
                    width:1
                }
            }

        }

        // InnerShadow{
        //     source: bgEffect
        //     anchors.fill:bgEffect
        //     radius: 8.0
        //     samples: 24
        //     color: "red"
        // }

        Item {
            id: actionArea
            Layout.preferredHeight: 96
            Layout.fillWidth: true

            ActionAreaBg {
                id: actionAreaBg
                anchors.fill: parent
            }

            //            TabBar {
            //                id: navLayout
            //                currentIndex: 0
            //                background: Item {}
            //                anchors.centerIn: parent

            //                NavBtn {
            //                    text: "Readings"
            //                    width: implicitWidth
            //                }
            //                NavBtn {
            //                    width: implicitWidth
            //                    text: "Config"
            //                }
            //            }
            RowLayout {
                id: actionRow
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.leftMargin: 16
                spacing: 50
                ActionBtn {
                    id: decreaseNominalFreqBtn
                    //anchors.verticalCenter: parent.verticalCenter
                    //anchors.left: parent.left
                    text: "1 Hz"
                    icon.source: "businesscontrols/icons/remove_black_24dp.svg"
                }

                ActionBtn {
                    id: quitAppBtn

                    //anchors.verticalCenter: parent.verticalCenter
                    text: "Quit"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    icon.source: "businesscontrols/icons/power_settings_new-24px.svg"
                }
                ActionBtn {
                    id: connectionBtn
                    checkable: true
                    text: "Conn"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    icon.source: "businesscontrols/icons/close_black_24dp.svg"
                }

                ActionBtn {
                    id: increaseNominalFreqBtn
                    //nchors.verticalCenter: parent.verticalCenter
                    //anchors.right: parent.right
                    text: "1 Hz"
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    icon.source: "businesscontrols/icons/add_black_24dp.svg"
                }


            }
        }
    }


    states: [
        State {
            name: "init"
            PropertyChanges {
                target: alertMsg
                visible: false
            }

            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: actualFreqReader
                state: ""
            }


            PropertyChanges {
                target: motorVoltReader
                state: ""
            }

            PropertyChanges {
                target: motorCurrReader
                state: ""
            }
        },
        State {
            name: "sensorDataTooOld"

            PropertyChanges {
                target: alertMsg
                visible: true
                state: "error"
                message: qsTr("Pressure and temperature values are too old.\nPlease check the connected device for any sensor-related issues.")
            }

            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: actualFreqReader
                state: ""
            }

            PropertyChanges {
                target: motorVoltReader
                state: ""
            }

            PropertyChanges {
                target: motorCurrReader
                state: ""
            }

        },
        State {
            name: "sensorDataOld"

            PropertyChanges {
                target: alertMsg
                visible: true
                state: "warning"
                message: "Pressure and temperature values are out of sync."
            }

            PropertyChanges {
                target: progressMsgArea
                visible: false
            }
        },
        State {
            name: "sensorDataRequesting"

            PropertyChanges {
                target: alertMsg
                visible: true
                color: ThemeConstants.colorInfo
                message: ""
            }

            PropertyChanges {
                target: progressMsg
                text: qsTr("Requesting sensor data...")
            }

            PropertyChanges {
                target: progressMsgArea
                visible: true
            }

            PropertyChanges {
                target: actualFreqReader
                state: ""
            }


            PropertyChanges {
                target: motorVoltReader
                state: ""
            }

            PropertyChanges {
                target: motorCurrReader
                state: ""
            }
        },
        State {
            name: "restarting"


            PropertyChanges {
                target: actualFreqReader
                state: "disconnected"
            }


            PropertyChanges {
                target: motorVoltReader
                state: "disconnected"
            }

            PropertyChanges {
                target: motorCurrReader
                state: "disconnected"
            }
            PropertyChanges {
                target: manualListenBtn
                enabled: false
            }

            PropertyChanges {
                target: actionArea
                enabled: false
            }

            PropertyChanges {
                target: detectionBox
                enabled: false
            }

            PropertyChanges {
                target: voiceMsgLabel
                text: root.language=="IT" ? "Resettando il sistema" : "Restarting system"
                font: Constants.fontMain
            }

            PropertyChanges {
                target: voiceMsgValue
                text: ""
            }

            PropertyChanges {
                target: msgArea
                color: Constants.colorError
            }
        }
    ]
}


