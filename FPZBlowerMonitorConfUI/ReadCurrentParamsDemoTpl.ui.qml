import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FPZBlowerMonitorConf 1.0
import "businesscontrols"
import "businesscontrols/themes"

Item {
    id: root
    width: Constants.width
    height: Constants.height
    property alias ch1ConfigForm: ch1ConfigForm
    property alias alertMsg: alertMsg
    property alias ambientTempReader: ambientTempReader
    property alias inletPressureReader: inletPressureReader

    property alias ambientPressureReader: ambientPressureReader
    property alias connectedDeviceName: connectedDeviceName
    state: "readCurrentParamsInit"
    property alias sensorDataLastUpdateTime: sensorDataLastUpdateTime
    property alias sensorDataUpdateIndicator: sensorDataUpdateIndicator
    property alias outletTempReader: outletTempReader
    property alias outletPressureReader: outletPressureReader
    property alias inletTempReader: inletTempReader

    TemplateBg {
        id: templateBg
        anchors.fill: parent
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 32

        StatusBar {
            id: statusArea
            Layout.fillWidth: true
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
                    color: Constants.colorMsgFg
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
                columns: 6

                ParamType {
                    id: paramTypePressure
                    paramLabel.text: "Pressure"
                    paramIcon.source: "businesscontrols/icons/gauge.svg"
                    Layout.preferredHeight: 180
                    Layout.preferredWidth: 40
                }

                ParamReaderCircle {
                    id: ambientPressureReader
                    minimumValue: ChannelMappingStore.ch2MinValue
                    maximumValue: ChannelMappingStore.ch2MaxValue
                    minWarningValue: ChannelMappingStore.ch2MinWarningValue
                    minErrorValue: ChannelMappingStore.ch2MinErrorValue
                    paramLabel.text: ChannelMappingStore.ch2Label
                    unitLabel.text: ChannelMappingStore.ch2Unit
                    paramIcon.source: "businesscontrols/icons/home.svg"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                ParamReader {
                    id: inletPressureReader
                    minWarningValue: 750
                    minErrorValue: 900
                    paramIcon.source: "businesscontrols/icons/import.svg"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    paramLabel.text: "Inlet"
                    unitLabel.text: "mbar"
                }

                ParamReader {
                    id: outletPressureReader
                    minWarningValue: 750
                    minErrorValue: 900
                    paramIcon.source: "businesscontrols/icons/export.svg"
                    paramLabel.text: "Outlet"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    unitLabel.text: "mbar"
                }

                ChConfigForm {
                    id: ch1ConfigForm
                    maxValue: ChannelMappingStore.ch1MaxValueInit
                    minValue: ChannelMappingStore.ch1MinValueInit
                    minErrorValue: ChannelMappingStore.ch1MinErrorValueInit
                    minWarningValue: ChannelMappingStore.ch1MinWarningValueInit
                    res: ChannelMappingStore.ch1ResInit
                    unit: ChannelMappingStore.ch1UnitInit
                    label: ChannelMappingStore.ch1LabelInit
                    chNum: "Channel 1"
                    Layout.rowSpan: 2
                    Connections {
                        target: ch1ConfigForm
                        onLabelChanged: ChannelMappingStore.ch1Label = ch1ConfigForm.label
                        onUnitChanged: ChannelMappingStore.ch1Unit = ch1ConfigForm.unit
                        onMinValueChanged: ChannelMappingStore.ch1MinValue = parseFloat(
                                               ch1ConfigForm.minValue)
                        onMinErrorValueChanged: ChannelMappingStore.ch1MinErrorValue = parseFloat(
                                                    ch1ConfigForm.minErrorValue)
                        onMinWarningValueChanged: ChannelMappingStore.ch1MinWarningValue
                                                  = parseFloat(
                                                      ch1ConfigForm.minWarningValue)
                        onMaxValueChanged: ChannelMappingStore.ch1MaxValue = parseFloat(
                                               ch1ConfigForm.maxValue)
                        onResChanged: ChannelMappingStore.ch1Res = parseInt(
                                          ch1ConfigForm.res)
                    }
                }

                ChConfigForm {
                    id: ch2ConfigForm
                    maxValue: ChannelMappingStore.ch2MaxValueInit
                    minValue: ChannelMappingStore.ch2MinValueInit
                    minErrorValue: ChannelMappingStore.ch2MinErrorValueInit
                    minWarningValue: ChannelMappingStore.ch2MinWarningValueInit
                    res: ChannelMappingStore.ch2ResInit
                    unit: ChannelMappingStore.ch2UnitInit
                    label: ChannelMappingStore.ch2LabelInit
                    chNum: "Channel 2"
                    Layout.rowSpan: 2
                    Connections {
                        target: ch2ConfigForm
                        onLabelChanged: ChannelMappingStore.ch2Label = ch2ConfigForm.label
                        onUnitChanged: ChannelMappingStore.ch2Unit = ch2ConfigForm.unit
                        onMinValueChanged: ChannelMappingStore.ch2MinValue = parseFloat(
                                               ch2ConfigForm.minValue)
                        onMaxValueChanged: ChannelMappingStore.ch2MaxValue = parseFloat(
                                               ch2ConfigForm.maxValue)
                        onMinErrorValueChanged: ChannelMappingStore.ch2MinErrorValue = parseFloat(
                                                    ch2ConfigForm.minErrorValue)
                        onMinWarningValueChanged: ChannelMappingStore.ch2MinWarningValue
                                                  = parseFloat(
                                                      ch2ConfigForm.minWarningValue)
                        onResChanged: ChannelMappingStore.ch2Res = parseInt(
                                          ch2ConfigForm.res)
                    }
                }

                ParamType {
                    id: paramTypeTemp
                    paramLabel.text: "Temp"
                    paramIcon.source: "businesscontrols/icons/thermometer.svg"
                    Layout.preferredHeight: 180
                    Layout.preferredWidth: 40
                }

                ParamReaderVert {
                    id: ambientTempReader
                    minimumValue: ChannelMappingStore.ch1MinValue
                    minWarningValue: ChannelMappingStore.ch1MinWarningValue
                    maximumValue: ChannelMappingStore.ch1MaxValue
                    minErrorValue: ChannelMappingStore.ch1MinErrorValue
                    unitLabel.text: ChannelMappingStore.ch1Unit
                    paramLabel.text: ChannelMappingStore.ch1Label
                    paramIcon.source: "businesscontrols/icons/home.svg"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                ParamReader {
                    id: inletTempReader
                    minWarningValue: 22
                    minErrorValue: 24
                    paramIcon.source: "businesscontrols/icons/import.svg"
                    paramLabel.text: "Inlet"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    unitLabel.text: "°C"
                }

                ParamReader {
                    id: outletTempReader
                    minWarningValue: 22
                    minErrorValue: 24
                    paramIcon.source: "businesscontrols/icons/export.svg"
                    paramLabel.text: "Outlet"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    unitLabel.text: "°C"
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

            PropertyChanges {
                target: ambientPressureReader
                state: ""
            }

            PropertyChanges {
                target: ambientTempReader
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

            PropertyChanges {
                target: ambientPressureReader
                state: ""
            }

            PropertyChanges {
                target: inletPressureReader
                state: ""
            }

            PropertyChanges {
                target: outletPressureReader
                state: ""
            }

            PropertyChanges {
                target: ambientTempReader
                state: ""
            }

            PropertyChanges {
                target: inletTempReader
                state: ""
            }

            PropertyChanges {
                target: outletTempReader
                state: ""
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
                color: Constants.colorInfo
            }

            PropertyChanges {
                target: sensorDataLastUpdateLabel
                color: Constants.colorInfo
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
                color: Constants.colorInfo
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

            PropertyChanges {
                target: ambientPressureReader
                state: ""
            }

            PropertyChanges {
                target: ambientTempReader
                state: ""
            }
        }
    ]
}



