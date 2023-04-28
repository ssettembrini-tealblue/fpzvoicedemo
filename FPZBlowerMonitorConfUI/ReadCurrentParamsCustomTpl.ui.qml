import QtQuick 2.15
import FPZBlowerMonitorConf 1.0
import QtQuick.Layouts 1.15
import "businesscontrols"
import "businesscontrols/themes"
import QtQuick.Controls 2.15

Item {
    id: root
    width: Constants.width
    height: Constants.height
    property alias customTripleReader: customTripleReader
    property alias customPressureReaderFlatBg: customPressureReaderFlatBg
    property alias alertMsg: alertMsg

    property alias connectedDeviceName: connectedDeviceName
    state: "readCurrentParamsInit"
    property alias sensorDataLastUpdateTime: sensorDataLastUpdateTime
    property alias sensorDataUpdateIndicator: sensorDataUpdateIndicator

    TemplateBg {
        id: templateBg
        anchors.fill: parent
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 32

        Item {
            id: statusArea
            Layout.preferredHeight: 80
            Layout.fillWidth: true

            StatusBarBg {
                id: statusAreaBg
                anchors.fill: parent
            }

            Row {
                id: sensorDataUpdateStatus
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 16
                spacing: 8
                SensorDataUpdateIndicator {
                    id: sensorDataUpdateIndicator
                }

                Text {
                    id: sensorDataLastUpdateTime
                    color: "#ffffff"
                    text: qsTr("0")
                    anchors.verticalCenter: parent.verticalCenter
                    font: Constants.fontMain
                }

                Text {
                    id: sensorDataLastUpdateLabel
                    color: "#ffffff"
                    text: qsTr("secs ago")
                    anchors.verticalCenter: parent.verticalCenter
                    font: Constants.fontMain
                }
            }

            Text {
                id: connectedDeviceName
                color: "#ffffff"
                text: qsTr("<connected device name>")
                anchors.verticalCenter: parent.verticalCenter
                font: Constants.fontMainM
                anchors.horizontalCenter: parent.horizontalCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            BtConnectionIndicator {
                id: btConnectionIndicator
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 16
                state: "connected"
            }
        }

        Item {
            id: msgArea
            Layout.preferredHeight: 64
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            AlertMsg {
                id: alertMsg
                width: 624
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
                    color: ThemeConstants.colorMsgFg
                    text: qsTr("<progress message>")
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

        Item {
            id: monitoringArea
            Layout.fillHeight: true
            Layout.fillWidth: true

            GridLayout {
                id: currentParametersGrid
                anchors.verticalCenter: parent.verticalCenter
                columnSpacing: 16
                rowSpacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                rows: 2
                columns: 5

                ReaderBg {
                    id: readerBox1
                    width: 180
                    height: 180
                }

                ReaderBox {
                    id: readerBox2

                    CustomRoundGauge {
                        id: customPressureReaderFlatBg
                        width: 160
                        height: 160
                        anchors.verticalCenter: parent.verticalCenter
                        showArc: true
                        arcSize: 0.6
                        labelInset: 16
                        anchors.horizontalCenter: parent.horizontalCenter
                        maximumValue: 900
                        end: 113
                        showNumbers: false
                        useFlatBg: true
                        begin: -135
                        paramLabel.text: "Ambient"
                        labelBottomMargin: 13
                    }
                }

                ReaderBox {
                    id: readerBox3
                    CustomTripleGauge {
                        id: customTripleReader
                        width: 160
                        height: 160
                        anchors.verticalCenter: parent.verticalCenter
                        leftGaugeTickmarkStepSize: 20
                        rightGaugeTickmarkStepSize: 200
                        rightGauge.tickmarksVisible: true
                        bottomGaugeUnit.text: "[%]"
                        bottomGaugeParam.text: "HR"
                        leftGaugeParam.text: "temp"
                        leftGaugeUnit.text: "[°C]"
                        rightGaugeParam.text: "p"
                        rightGaugeUnit.text: "[mbar]"
                        bottomGauge.from: 0
                        bottomGauge.to: 100
                        rightGauge.maximumValue: 1000
                        leftGauge.minimumValue: 0
                        leftGauge.maximumValue: 100
                        rightGaugeBegin: 125
                        leftGaugeBegin: -125
                        showNumbers: true
                        leftGauge.tickmarksVisible: true
                        paramLabel.text: "Ambient"
                        anchors.horizontalCenter: parent.horizontalCenter
                        labelBottomMargin: 4
                        labelInset: 24
                    }
                }

                CustomRoundGauge {
                    id: customRoundGauge
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

            TabBar {
                id: navLayout
                currentIndex: 0
                background: Item {}
                anchors.centerIn: parent

                NavBtn {
                    text: "Now"
                    width: implicitWidth
                }
                NavBtn {
                    width: implicitWidth
                    text: "Today"
                }

                NavBtn {
                    width: implicitWidth
                    text: "Week"
                }

                NavBtn {
                    width: implicitWidth
                    text: "Month"
                }
            }

            PowerBtn {
                id: powerBtn
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
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
                target: sensorDataUpdateStatus
                visible: false
            }

            PropertyChanges {
                target: connectedDeviceName
                visible: false
            }

            PropertyChanges {
                target: btConnectionIndicator
                visible: false
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
                target: sensorDataUpdateStatus
                visible: true
            }

            PropertyChanges {
                target: connectedDeviceName
                visible: true
            }

            PropertyChanges {
                target: btConnectionIndicator
                visible: true
            }

            PropertyChanges {
                target: sensorDataUpdateIndicator
                state: "error"
            }

            PropertyChanges {
                target: sensorDataLastUpdateTime
                color: Constants.colorError
            }

            PropertyChanges {
                target: sensorDataLastUpdateLabel
                color: Constants.colorError
            }
        },
        State {
            name: "sensorDataSyncing"

            PropertyChanges {
                target: sensorDataUpdateIndicator
                state: "running"
            }

            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: alertMsg
                visible: false
            }

            PropertyChanges {
                target: sensorDataUpdateStatus
                visible: true
            }

            PropertyChanges {
                target: connectedDeviceName
                visible: true
            }

            PropertyChanges {
                target: btConnectionIndicator
                visible: true
            }

            PropertyChanges {
                target: sensorDataLastUpdateTime
                color: ThemeConstants.colorInfo
            }

            PropertyChanges {
                target: sensorDataLastUpdateLabel
                color: ThemeConstants.colorInfo
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

            PropertyChanges {
                target: sensorDataUpdateStatus
                visible: true
            }

            PropertyChanges {
                target: connectedDeviceName
                visible: true
            }

            PropertyChanges {
                target: btConnectionIndicator
                visible: true
            }

            PropertyChanges {
                target: sensorDataUpdateIndicator
                state: "alert"
            }

            PropertyChanges {
                target: sensorDataLastUpdateTime
                color: "#be7502"
            }

            PropertyChanges {
                target: sensorDataLastUpdateLabel
                color: "#be7502"
            }
        },
        State {
            name: "sensorDataRequesting"
            PropertyChanges {
                target: sensorDataUpdateIndicator
                state: "running"
            }

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
                target: sensorDataUpdateStatus
                visible: false
            }

            PropertyChanges {
                target: progressMsgArea
                visible: true
            }

            PropertyChanges {
                target: connectedDeviceName
                visible: true
            }

            PropertyChanges {
                target: btConnectionIndicator
                visible: true
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

