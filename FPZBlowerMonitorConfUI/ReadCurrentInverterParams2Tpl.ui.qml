import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FPZBlowerMonitorConf 1.0
import "businesscontrols"
import "businesscontrols/themes"

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
    property alias decreaseNominalFreqBtn: decreaseNominalFreqBtn
    property alias increaseNominalFreqBtn: increaseNominalFreqBtn
    property alias actionRow: actionRow

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
        }

        Item {
            id: msgArea
            Layout.preferredHeight: 96
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            AlertMsg {
                id: alertMsg
                y: 56
                width: 572
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Row {
                id: progressMsgArea
                width: 240
                anchors.verticalCenter: parent.verticalCenter
                spacing: 32
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    id: progressMsg
                    color: Constants.colorMsgFg
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

        StackLayout {
            id: contentArea
            width: 100
            height: 80
            currentIndex: navLayout.currentIndex
            //currentIndex: 1
            Layout.fillHeight: true
            Layout.fillWidth: true

            Item {
                id: monitoringPage
                Layout.fillHeight: true
                Layout.fillWidth: true

                GridLayout {
                    id: currentParametersGrid
                    anchors.verticalCenter: parent.verticalCenter
                    columnSpacing: 16
                    rowSpacing: 16
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
                    icon.source: "businesscontrols/icons/close_black_24dp.svg"
                }
                ActionBtn {
                    id: connectionBtn
                    checkable: true
                    text: "Conn"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    icon.source: "businesscontrols/icons/power_settings_new-24px.svg"
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
                target: currentReader
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
                color: Constants.colorInfo
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
        }
    ]
}
