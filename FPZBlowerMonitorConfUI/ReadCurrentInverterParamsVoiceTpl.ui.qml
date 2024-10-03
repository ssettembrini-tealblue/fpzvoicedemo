import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import FPZBlowerMonitorConf 1.0
import QtGraphicalEffects 1.12
import "./businesscontrols"
import "./businesscontrols/themes"

Item {
    id: root
    width: Constants.width
    height: Constants.contentHeight
    property alias startDO2Btn: startDO2Btn
    property alias stopDO2Btn: stopDO2Btn
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
    property alias languageIndicator: languageIndicator
    property alias changeLanguageBtn: changeLanguageBtn
    property alias checkLanguageBtn: checkLanguageBtn
    property alias manualDetectionTimer: manualDetectionTimer
    property alias inject: injectMsg
    property alias txtCounterMsg: txtCounterMsg
    property alias txtWebsocketValid: txtWebsocketValid
    property alias infoOverlayRegion: infoOverlayRegion
    property alias infoOverlay: infoOverlay
    property alias infoBtn: infoBtn
    property alias toggleDO2Btn: toggleDO2Btn
    //property alias pidDebug: pidDebug
    property bool detectedWakeWord: false

    property bool activeCommand
    property string listenedCommand
    property string commandName
    property int commandValue
    property string language
    property string debug
    property bool visibleBox: false

    Timer {
        id: manualDetectionTimer
        interval: 3000
        running: false
    }

    TemplateBg {
        id: templateBg
        anchors.fill: parent
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        StatusBar {
            id: statusBar
            Layout.fillWidth: true
            btConnectionIndicator.visible: false
            modbusConnectionIndicator.visible: true
            sensorDataUpdateStatus.visible: false
            voiceConnectionIndicator.visible: true
            wakewordDetectedIndicator.visible: true
            wakewordDetectedIndicator.detected: root.detectedWakeWord
        }

        // Item {

        //     Layout.preferredHeight: 96
        //     Layout.fillWidth: true
        //     Layout.alignment: Qt.AlignHCenter //| Qt.AlignVCenter
        Rectangle {
            id: bgEffect
            color: "transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true
            ColumnLayout {
                id: columnLayoutContent
                anchors.fill: parent

                Item {
                    Layout.fillHeight: true
                    width: 1
                }

                Row {
                    spacing: 10
                    //anchors.horizontalCenter: parent.horizontalCenter
                    //anchors.top: parent.top
                    //anchors.topMargin: 80
                    Layout.alignment: Qt.AlignHCenter
                    Rectangle {
                        id: detectionBox
                        visible: false
                        anchors.verticalCenter: parent.verticalCenter
                        width: 180
                        height: 40
                        radius: 12

                        color: enabled ? (root.detectedWakeWord ? "#a0db8e" : "#ff6f69") : "#cccccc"
                        opacity: enabled ? 1 : 0.8
                        Row {
                            anchors.centerIn: parent

                            // Rectangle{
                            //     height: 10
                            //     width: 10
                            //     radius: height/2
                            //     color:root.detectedWakeWord ? "green" : "red"
                            //     anchors.verticalCenter: parent.verticalCenter
                            // }
                            Text {
                                id: txtWakeWord
                                text: root.detectedWakeWord ? "DETECTED" : "NOT DETECTED"
                                color: "black" //root.detectedWakeWord ? "green" : "red"
                                anchors.verticalCenter: parent.verticalCenter
                                //font.pixelSize: 24
                                //lineHeight: 1.2
                                opacity: 0.5
                                font: Constants.fontMainS
                            }
                            Text {
                                text: "-"
                                visible: txtDebug.visible
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            Text {
                                id: txtDebug
                                visible: false
                                text: root.debug
                                color: "black" //root.detectedWakeWord ? "green" : "red"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                    // Item {
                    //     width: 30
                    //     height: 1
                    // }
                    ActionBtn {
                        visible: false
                        id: startDO2Btn
                        text: "Start DO2"
                    }
                    ActionBtn {
                        visible: false
                        id: stopDO2Btn
                        text: "Stop DO2"
                    }
                    ActionBtn {
                        id: checkLanguageBtn
                        visible: false
                        text: "Check"
                        icon.source: "businesscontrols/icons/add_black_24dp.svg"
                    }
                    ActionBtn {
                        id: changeStateBtn
                        visible: false
                        text: "Change"
                        icon.source: "businesscontrols/icons/add_black_24dp.svg"
                    }
                    ActionBtn {
                        id: injectMsg
                        visible: false
                        text: "Inject"
                        icon.source: "businesscontrols/icons/add_black_24dp.svg"
                    }
                    Column {

                        visible: false
                        anchors.verticalCenter: parent.verticalCenter

                        Text {
                            id: txtCounterMsg
                            text: "Counter: " + voiceStore.msgCounter
                            color: "black" //root.detectedWakeWord ? "green" : "red"
                        }
                        Text {
                            text: voiceStore.reachableMycroft ? "MYCROFT REACHABLE" : "MYCROFT UNREACHABLE"
                            id: txtReachableMycroft
                        }
                        Text {
                            id: txtRestarting
                            text: voiceStore.restarting ? "RESTARTING" : "NOT RESTARTING"
                            color: "black"
                        }
                        Text {
                            id: txtWebsocketValid
                            text: voiceStore.websocketValid ? "WS ACTIVE" : "WS DISCONN"
                            color: "black"
                        }

                        Text {
                            id: txtReachableSkills
                            text: voiceStore.reachableSkills ? "ACTIVE SKILL" : "INACTIVE SKILL"
                            color: "black" //root.detectedWakeWord ? "green" : "red"
                        }
                        Text {

                            text: root.debug
                            color: "black"
                        }
                    }
                }

                Item {
                    Layout.preferredHeight: 16
                    width: 1
                }
                Rectangle {
                    color: "transparent"
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: currentParametersGrid.implicitWidth
                    Layout.minimumWidth: currentParametersGrid.implicitWidth
                    Layout.minimumHeight: 72
                    Layout.preferredHeight: 72
                    Rectangle {
                        id: msgArea
                        visible: root.visibleBox
                        width: parent.width
                        radius: 10 //16
                        height: parent.height
                        color: ThemeConstants.colorInfo

                        Column {
                            anchors.centerIn: parent
                            Text {
                                id: voiceMsgLabel
                                //visible: !root.activeCommand
                                color: "#F5F4F4" //"#5b5b5b"//ThemeConstants.colorMsgFg
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
                                color: "#F5F4F4" //"#5b5b5b"//ThemeConstants.colorMsgFg
                                text: root.activeCommand ? root.commandName + (root.commandValue != 0 ? " - " + root.commandValue.toString() : "") : "\"" + root.listenedCommand + "\""
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
                            visible: false
                            y: 56
                            width: 572
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Row {
                            id: progressMsgArea
                            visible: false
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
                }

                Item {
                    Layout.preferredHeight: 16
                    width: 1
                }
                Item {
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
                            maximumValue: 75
                            minWarningValue: 30
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
                Item {
                    Layout.fillHeight: true
                    width: 1
                }
            }
        }

        Item {
            id: actionArea
            Layout.preferredHeight: 96
            Layout.fillWidth: true

            ActionAreaBg {
                id: actionAreaBg
                anchors.fill: parent
            }


            RowLayout {
                id: actionRow
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 24 //16
                anchors.leftMargin: 24 //16
                //spacing: 18 //40//50
                InfoBtn {
                    id: infoBtn
                    text: "Info" //""
                    Layout.alignment: Qt.AlignVCenter

                }
                ActionBtn {
                    id: decreaseNominalFreqBtn
                    Layout.alignment: Qt.AlignVCenter
                    text: "1 Hz"
                    icon.source: "businesscontrols/icons/remove_black_24dp.svg"
                }
                ActionBtn {
                    id: increaseNominalFreqBtn
                    text: "1 Hz"
                    Layout.alignment: Qt.AlignVCenter
                    icon.source: "businesscontrols/icons/add_black_24dp.svg"
                }

                Button {
                    id: manualListenBtn

                    Layout.alignment: Qt.AlignVCenter
                    // anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: enabled ? -2 : 0
                    //height: 32
                    width: 180
                    leftPadding: 15
                    rightPadding: 15
                    display: icon.source
                             === "" ? AbstractButton.TextBesideIcon : AbstractButton.TextOnly
                    //icon.source: "icons/power_settings_new-24px.svg"
                    background: ActionBtnBg {
                        down: manualListenBtn.pressed
                        width: parent.width
                        //height:32
                    }
                    contentItem: Row {
                        opacity: enabled ? 1 : 0.5
                        spacing: 8
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: manualListenBtn.pressed ? 2 : 0
                        Text {

                            text: "Manual Listen"
                            font.capitalization: Font.AllUppercase
                            font.family: Constants.fontMain.family
                            font.weight: Constants.fontMain.weight
                            font.pixelSize: 13
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Rectangle {
                            height: 10
                            width: 10
                            radius: height / 2
                            color: enabled ? "red" : "grey"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
                ActionBtn {
                    id: connectionBtn
                    visible: false
                    checkable: true
                    text: "Conn"
                    Layout.alignment: Qt.AlignVCenter // Qt.AlignHCenter | Qt.AlignVCenter
                    icon.source: "businesscontrols/icons/close_black_24dp.svg"
                }
                ActionBtn {
                    id: toggleDO2Btn

                    checkable: true
                    text: "toggle do2"
                    Layout.alignment: Qt.AlignVCenter //Qt.AlignHCenter | Qt.AlignVCenter
                    icon.source: "businesscontrols/icons/close_black_24dp.svg"
                }
                ActionBtn {
                    id: quitAppBtn
                    text: "Quit"
                    Layout.alignment: Qt.AlignVCenter
                    icon.source: "businesscontrols/icons/power_settings_new-24px.svg"
                }
                Button {
                    id: changeLanguageBtn
                    //anchors.right: parent.right
                    //anchors.rightMargin: 24
                    Layout.alignment: Qt.AlignVCenter
                    anchors.verticalCenterOffset: enabled ? -2 : 0
                    //anchors.verticalCenter: parent.verticalCenter
                    width: 74 //rowLanguage.childrenRect.width + 40//72
                    Layout.preferredWidth: 74
                    background: ActionBtnBg {
                        width: 74 //sparent.width //72
                        // height: 52
                        down: changeLanguageBtn.pressed
                    }
                    contentItem: // Item{
                                 // anchors.verticalCenter: parent.verticalCenter
                                 // anchors.horizontalCenter: parent.horizontalCenter

                                 Row {
                        id: rowLanguage

                        spacing: 2
                        //x:8
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        Item {
                            visible: false
                            height: 1
                            width: 4
                        }

                        Image {
                            id: image
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.verticalCenterOffset: changeLanguageBtn.pressed ? 2 : 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: true
                            source: language == "IT" ? "./businesscontrols/icons/gb.svg" : "./businesscontrols/icons/it.svg"
                            height: 36
                            width: 42
                            opacity: (!voiceStore.reachableSkills
                                      || !voiceStore.reachableMycroft
                                      || !voiceStore.reachableStt) ? 0.1 : 1

                            sourceSize.height: height
                            sourceSize.width: width
                            fillMode: Image.PreserveAspectFit
                            layer.enabled: true
                            layer.effect: OpacityMask {

                                maskSource: Item {
                                    height: image.height
                                    width: image.width
                                    Rectangle {
                                        id: imgMask
                                        anchors.centerIn: parent

                                        //color: "white"
                                        opacity: 1 //0.85
                                        height: 36
                                        width: 36
                                        radius: 18
                                    }
                                }
                            }
                        }

                        Item {
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
                            text: language == "IT" ? "CAMBIA LINGUA" : "CHANGE LANGUAGE"
                            font.family: Constants.fontMainS.family
                            font.weight: Constants.fontMainS.weight
                            font.pixelSize: 20 //Constants.fontMainM.pixelSize
                            color: "black"
                        }
                    }
                    Rectangle {
                        visible: (!voiceStore.reachableSkills
                                  || !voiceStore.reachableMycroft
                                  || !voiceStore.reachableStt)

                        x: 20 //11
                        y: 10
                        color: "grey"
                        opacity: 0.4
                        height: 36
                        width: 36
                        radius: 18
                    }
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
                target: msgArea
                visible: visibleBox
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
                state: "active"
            }

            PropertyChanges {
                target: motorVoltReader
                state: "active"
            }

            PropertyChanges {
                target: motorCurrReader
                state: "active"
            }
            PropertyChanges {
                target: msgArea
                visible: visibleBox
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
                state: "warning"
            }

            PropertyChanges {
                target: motorVoltReader
                state: "warning"
            }

            PropertyChanges {
                target: motorCurrReader
                state: "warning"
            }
            PropertyChanges {
                target: msgArea
                visible: visibleBox
            }
        },
        State {
            name: "restarting"

            PropertyChanges {
                target: changeLanguageBtn
                enabled: false
            }

            PropertyChanges {
                target: manualListenBtn
                enabled: false
            }

            PropertyChanges {
                target: detectionBox
                enabled: false
            }

            PropertyChanges {
                target: voiceMsgLabel
                text: root.language == "IT" ? "Riavviando il sistema" : "Restarting system"
                font: Constants.fontMain
            }

            PropertyChanges {
                target: voiceMsgValue
                text: ""
            }

            PropertyChanges {
                target: msgArea
                color: ThemeConstants.colorInfo
                visible: true
            }
        },
        State {
            name: "inactive"

            PropertyChanges {
                target: changeLanguageBtn
                enabled: false
            }

            PropertyChanges {
                target: manualListenBtn
                enabled: false
            }

            PropertyChanges {
                target: detectionBox
                enabled: false
            }

            PropertyChanges {
                target: voiceMsgLabel
                text: root.language == "IT" ? "Sistema inattivo" : "Inactive system"
                font: Constants.fontMain
            }

            PropertyChanges {
                target: voiceMsgValue
                text: ""
            }

            PropertyChanges {
                target: msgArea
                color: Constants.colorError
                visible: true
            }
        },
        State {
            name: "inactiveskill"

            PropertyChanges {
                target: changeLanguageBtn
                enabled: false
            }

            PropertyChanges {
                target: manualListenBtn
                enabled: false
            }

            PropertyChanges {
                target: detectionBox
                enabled: false
            }

            PropertyChanges {
                target: voiceMsgLabel
                text: root.language == "IT" ? "Avvio skill" : "Activating skill"
                font: Constants.fontMain
            }

            PropertyChanges {
                target: voiceMsgValue
                text: ""
            }

            PropertyChanges {
                target: msgArea
                color: ThemeConstants.colorInfo
                visible: true
            }
        }
    ]

    Rectangle {
        id: filler
        visible: false
    }

    Popup {
        id: infoOverlayRegion
        //visible: false
        anchors.centerIn: parent
        implicitWidth: infoOverlay.width
        implicitHeight: infoOverlay.height
        modal: true
        focus: true
        padding: 0
        contentItem: Item {
            InfoOverlayTpl {
                id: infoOverlay
                visible: true
                anchors.fill: parent
            }
        }
        background: Item {}
        onClosed: infoBtn.checked = false
    }
}
