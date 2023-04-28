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
    property alias powerBtn: powerBtn
    property alias actionArea: actionArea
    property alias alertMsg: alertMsg
    property alias ch4Reader: ch4Reader
    property alias ch2Reader: ch2Reader

    property alias ch1Reader: ch1Reader
    property alias ch6Reader: ch6Reader
    property alias ch3Reader: ch3Reader
    property alias ch5Reader: ch5Reader

    TemplateBg {
        id: templateBg
        anchors.fill: parent
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 32

        Item {
            id: msgArea
            Layout.preferredHeight: 96
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            AlertMsg {
                id: alertMsg
                width: 624
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: -1
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

        StackLayout {
            id: contentArea
            width: 100
            height: 100
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
                    rows: 2
                    columns: 4

                    ParamType {
                        id: paramTypePressure
                        paramLabel.text: "Pressure"
                        paramIcon.source: "businesscontrols/icons/gauge.svg"
                        Layout.preferredHeight: 180
                        Layout.preferredWidth: 40
                    }

                    ParamReaderCircle {
                        id: ch1Reader
                        minimumValue: ChannelMappingStore.ch1MinValue
                        maximumValue: ChannelMappingStore.ch1MaxValue
                        minWarningValue: ChannelMappingStore.ch1MinWarningValue
                        minErrorValue: ChannelMappingStore.ch1MinErrorValue
                        paramLabel.text: ChannelMappingStore.ch1Label
                        unitLabel.text: ChannelMappingStore.ch1Unit
                        paramIcon.source: "businesscontrols/icons/"
                                          + ChannelMappingStore.ch1Icon + ".svg"

                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    ParamReader {
                        id: ch2Reader
                        minWarningValue: ChannelMappingStore.ch2MinWarningValue
                        minErrorValue: ChannelMappingStore.ch2MinErrorValue
                        paramLabel.text: ChannelMappingStore.ch2Label
                        unitLabel.text: ChannelMappingStore.ch2Unit
                        paramIcon.source: "businesscontrols/icons/"
                                          + ChannelMappingStore.ch2Icon + ".svg"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    ParamReader {
                        id: ch3Reader
                        minWarningValue: ChannelMappingStore.ch3MinWarningValue
                        minErrorValue: ChannelMappingStore.ch3MinErrorValue
                        paramLabel.text: ChannelMappingStore.ch3Label
                        unitLabel.text: ChannelMappingStore.ch3Unit
                        paramIcon.source: "businesscontrols/icons/"
                                          + ChannelMappingStore.ch3Icon + ".svg"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    ParamType {
                        id: paramTypeTemp
                        paramLabel.text: "Temp"
                        paramIcon.source: "businesscontrols/icons/thermometer.svg"
                        Layout.preferredHeight: 180
                        Layout.preferredWidth: 40
                    }

                    ParamReaderVert {
                        id: ch4Reader
                        minimumValue: ChannelMappingStore.ch4MinValue
                        minWarningValue: ChannelMappingStore.ch4MinWarningValue
                        maximumValue: ChannelMappingStore.ch4MaxValue
                        minErrorValue: ChannelMappingStore.ch4MinErrorValue
                        unitLabel.text: ChannelMappingStore.ch4Unit
                        paramLabel.text: ChannelMappingStore.ch4Label
                        paramIcon.source: "businesscontrols/icons/"
                                          + ChannelMappingStore.ch4Icon + ".svg"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    ParamReader {
                        id: ch5Reader
                        minWarningValue: ChannelMappingStore.ch5MinWarningValue
                        minErrorValue: ChannelMappingStore.ch5MinErrorValue
                        unitLabel.text: ChannelMappingStore.ch5Unit
                        paramLabel.text: ChannelMappingStore.ch5Label
                        paramIcon.source: "businesscontrols/icons/"
                                          + ChannelMappingStore.ch5Icon + ".svg"

                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    ParamReader {
                        id: ch6Reader
                        minWarningValue: ChannelMappingStore.ch6MinWarningValue
                        minErrorValue: ChannelMappingStore.ch6MinErrorValue
                        unitLabel.text: ChannelMappingStore.ch6Unit
                        paramLabel.text: ChannelMappingStore.ch6Label
                        paramIcon.source: "businesscontrols/icons/"
                                          + ChannelMappingStore.ch6Icon + ".svg"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                }
            }

            Item {
                id: configPage1
                Layout.fillHeight: true
                Layout.fillWidth: true
                GridLayout {
                    anchors.verticalCenter: parent.verticalCenter
                    rowSpacing: 16
                    columnSpacing: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    columns: 6

                    ChConfigForm {
                        id: ch1ConfigForm
                        res: ChannelMappingStore.ch1ResInit
                        minWarningValue: ChannelMappingStore.ch1MinWarningValueInit
                        unit: ChannelMappingStore.ch1UnitInit
                        icon: ChannelMappingStore.ch1IconInit
                        sensorType: ChannelMappingStore.ch1SensorTypeInit
                        Layout.rowSpan: 2
                        label: ChannelMappingStore.ch1LabelInit
                        minErrorValue: ChannelMappingStore.ch1MinErrorValueInit
                        Connections {
                            target: ch1ConfigForm
                            onLabelChanged: ChannelMappingStore.ch1Label = ch1ConfigForm.label
                            onUnitChanged: ChannelMappingStore.ch1Unit = ch1ConfigForm.unit
                            onIconChanged: ChannelMappingStore.ch1Icon = ch1ConfigForm.icon
                            onMinValueChanged: ChannelMappingStore.ch1MinValue
                                               = ch1ConfigForm.minValue
                            onMinWarningValueChanged: ChannelMappingStore.ch1MinWarningValue
                                                      = ch1ConfigForm.minWarningValue
                            onMinErrorValueChanged: ChannelMappingStore.ch1MinErrorValue
                                                    = ch1ConfigForm.minErrorValue
                            onMaxValueChanged: ChannelMappingStore.ch1MaxValue
                                               = ch1ConfigForm.maxValue
                            onResChanged: ChannelMappingStore.ch1Res = ch1ConfigForm.res
                            onSensorTypeChanged: ChannelMappingStore.ch1SensorType = ch1ConfigForm.sensorType

                        }
                        maxValue: ChannelMappingStore.ch1MaxValueInit
                        chNum: "Channel 1"
                        minValue: ChannelMappingStore.ch1MinValueInit
                    }

                    ChConfigForm {
                        id: ch2ConfigForm
                        res: ChannelMappingStore.ch2ResInit
                        minWarningValue: ChannelMappingStore.ch2MinWarningValueInit
                        unit: ChannelMappingStore.ch2UnitInit
                        icon: ChannelMappingStore.ch2IconInit
                        sensorType: ChannelMappingStore.ch2SensorTypeInit
                        Layout.rowSpan: 2
                        label: ChannelMappingStore.ch2LabelInit
                        minErrorValue: ChannelMappingStore.ch2MinErrorValueInit
                        Connections {
                            target: ch2ConfigForm
                            onLabelChanged: ChannelMappingStore.ch2Label = ch2ConfigForm.label
                            onUnitChanged: ChannelMappingStore.ch2Unit = ch2ConfigForm.unit
                            onIconChanged: ChannelMappingStore.ch2Icon = ch2ConfigForm.icon
                            onMinValueChanged: ChannelMappingStore.ch2MinValue
                                               = ch2ConfigForm.minValue
                            onMinWarningValueChanged: ChannelMappingStore.ch2MinWarningValue
                                                      = ch2ConfigForm.minWarningValue
                            onMinErrorValueChanged: ChannelMappingStore.ch2MinErrorValue
                                                    = ch2ConfigForm.minErrorValue
                            onMaxValueChanged: ChannelMappingStore.ch2MaxValue
                                               = ch2ConfigForm.maxValue
                            onResChanged: ChannelMappingStore.ch2Res = ch2ConfigForm.res
                            onSensorTypeChanged: ChannelMappingStore.ch2SensorType = ch2ConfigForm.sensorType

                        }
                        maxValue: ChannelMappingStore.ch2MaxValueInit
                        chNum: "Channel 2"
                        minValue: ChannelMappingStore.ch2MinValueInit
                    }
                    rows: 2
                }
            }

            Item {
                id: configPage2
                GridLayout {
                    anchors.verticalCenter: parent.verticalCenter
                    rows: 2
                    columnSpacing: 16
                    anchors.horizontalCenter: parent.horizontalCenter

                    ChConfigForm {
                        id: ch3ConfigForm
                        res: ChannelMappingStore.ch3ResInit
                        maxValue: ChannelMappingStore.ch3MaxValueInit
                        unit: ChannelMappingStore.ch3UnitInit
                        icon: ChannelMappingStore.ch3IconInit
                        sensorType: ChannelMappingStore.ch3SensorTypeInit
                        minWarningValue: ChannelMappingStore.ch3MinWarningValueInit
                        Layout.rowSpan: 2
                        chNum: "Channel 3"
                        Connections {
                            target: ch3ConfigForm
                            onLabelChanged: ChannelMappingStore.ch3Label = ch3ConfigForm.label
                            onUnitChanged: ChannelMappingStore.ch3Unit = ch3ConfigForm.unit
                            onIconChanged: ChannelMappingStore.ch3Icon = ch3ConfigForm.icon

                            onMinValueChanged: ChannelMappingStore.ch3MinValue
                                               = ch3ConfigForm.minValue
                            onMinWarningValueChanged: ChannelMappingStore.ch3MinWarningValue
                                                      = ch3ConfigForm.minWarningValue
                            onMinErrorValueChanged: ChannelMappingStore.ch3MinErrorValue
                                                    = ch3ConfigForm.minErrorValue
                            onMaxValueChanged: ChannelMappingStore.ch3MaxValue
                                               = ch3ConfigForm.maxValue
                            onResChanged: ChannelMappingStore.ch3Res = ch3ConfigForm.res
                            onSensorTypeChanged: ChannelMappingStore.ch3SensorType = ch3ConfigForm.sensorType

                        }
                        label: ChannelMappingStore.ch3LabelInit
                        minValue: ChannelMappingStore.ch3MinValueInit
                        minErrorValue: ChannelMappingStore.ch3MinErrorValueInit
                    }

                    ChConfigForm {
                        id: ch4ConfigForm
                        res: ChannelMappingStore.ch4ResInit
                        maxValue: ChannelMappingStore.ch4MaxValueInit
                        unit: ChannelMappingStore.ch4UnitInit
                        icon: ChannelMappingStore.ch4IconInit
                        sensorType: ChannelMappingStore.ch4SensorTypeInit
                        minWarningValue: ChannelMappingStore.ch4MinWarningValueInit
                        Layout.rowSpan: 2
                        chNum: "Channel 4"
                        Connections {
                            target: ch4ConfigForm
                            onLabelChanged: ChannelMappingStore.ch4Label = ch4ConfigForm.label
                            onUnitChanged: ChannelMappingStore.ch4Unit = ch4ConfigForm.unit
                            onIconChanged: ChannelMappingStore.ch4Icon = ch4ConfigForm.icon
                            onMinValueChanged: ChannelMappingStore.ch4MinValue
                                               = ch4ConfigForm.minValue
                            onMinWarningValueChanged: ChannelMappingStore.ch4MinWarningValue
                                                      = ch4ConfigForm.minWarningValue
                            onMinErrorValueChanged: ChannelMappingStore.ch4MinErrorValue
                                                    = ch4ConfigForm.minErrorValue
                            onMaxValueChanged: ChannelMappingStore.ch4MaxValue
                                               = ch4ConfigForm.maxValue
                            onResChanged: ChannelMappingStore.ch4Res = ch4ConfigForm.res
                            onSensorTypeChanged: ChannelMappingStore.ch4SensorType = ch4ConfigForm.sensorType

                        }
                        label: ChannelMappingStore.ch4LabelInit
                        minValue: ChannelMappingStore.ch4MinValueInit
                        minErrorValue: ChannelMappingStore.ch4MinErrorValueInit
                    }
                    rowSpacing: 16
                    columns: 6
                }
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Item {
                id: configPage3
                GridLayout {
                    anchors.verticalCenter: parent.verticalCenter
                    rows: 2
                    columnSpacing: 16
                    anchors.horizontalCenter: parent.horizontalCenter

                    ChConfigForm {
                        id: ch5ConfigForm
                        res: ChannelMappingStore.ch5ResInit
                        maxValue: ChannelMappingStore.ch5MaxValueInit
                        unit: ChannelMappingStore.ch5UnitInit
                        icon: ChannelMappingStore.ch5IconInit
                        sensorType: ChannelMappingStore.ch5SensorTypeInit
                        minWarningValue: ChannelMappingStore.ch5MinWarningValueInit
                        Layout.rowSpan: 2
                        chNum: "Channel 5"
                        Connections {
                            target: ch5ConfigForm
                            onLabelChanged: ChannelMappingStore.ch5Label = ch5ConfigForm.label
                            onUnitChanged: ChannelMappingStore.ch5Unit = ch5ConfigForm.unit
                            onIconChanged: ChannelMappingStore.ch5Icon = ch5ConfigForm.icon
                            onMinValueChanged: ChannelMappingStore.ch5MinValue
                                               = ch5ConfigForm.minValue
                            onMinWarningValueChanged: ChannelMappingStore.ch5MinWarningValue
                                                      = ch5ConfigForm.minWarningValue
                            onMinErrorValueChanged: ChannelMappingStore.ch5MinErrorValue
                                                    = ch5ConfigForm.minErrorValue
                            onMaxValueChanged: ChannelMappingStore.ch5MaxValue
                                               = ch5ConfigForm.maxValue
                            onResChanged: ChannelMappingStore.ch5Res = ch5ConfigForm.res
                            onSensorTypeChanged: ChannelMappingStore.ch5SensorType = ch5ConfigForm.sensorType
                        }
                        label: ChannelMappingStore.ch5LabelInit
                        minValue: ChannelMappingStore.ch5MinValueInit
                        minErrorValue: ChannelMappingStore.ch5MinErrorValueInit
                    }

                    ChConfigForm {
                        id: ch6ConfigForm
                        res: ChannelMappingStore.ch6ResInit
                        maxValue: ChannelMappingStore.ch6MaxValueInit
                        unit: ChannelMappingStore.ch6UnitInit
                        icon: ChannelMappingStore.ch6IconInit
                        sensorType: ChannelMappingStore.ch6SensorTypeInit
                        minWarningValue: ChannelMappingStore.ch6MinWarningValueInit
                        Layout.rowSpan: 2
                        chNum: "Channel 6"
                        Connections {
                            target: ch6ConfigForm
                            onLabelChanged: ChannelMappingStore.ch6Label = ch6ConfigForm.label
                            onUnitChanged: ChannelMappingStore.ch6Unit = ch6ConfigForm.unit
                            onIconChanged: ChannelMappingStore.ch6Icon = ch6ConfigForm.icon
                            onMinValueChanged: ChannelMappingStore.ch6MinValue
                                               = ch6ConfigForm.minValue
                            onMinWarningValueChanged: ChannelMappingStore.ch6MinWarningValue
                                                      = ch6ConfigForm.minWarningValue
                            onMinErrorValueChanged: ChannelMappingStore.ch6MinErrorValue
                                                    = ch6ConfigForm.minErrorValue
                            onMaxValueChanged: ChannelMappingStore.ch6MaxValue
                                               = ch6ConfigForm.maxValue
                            onResChanged: ChannelMappingStore.ch6Res = ch6ConfigForm.res
                            onSensorTypeChanged: ChannelMappingStore.ch6SensorType = ch6ConfigForm.sensorType

                        }
                        label: ChannelMappingStore.ch6LabelInit
                        minValue: ChannelMappingStore.ch6MinValueInit
                        minErrorValue: ChannelMappingStore.ch6MinErrorValueInit
                    }
                    rowSpacing: 16
                    columns: 6
                }
                Layout.fillHeight: true
                Layout.fillWidth: true
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
                anchors.verticalCenter: parent.verticalCenter
                currentIndex: 0
                background: Item {}

                NavBtn {
                    text: "Readings"
                    width: implicitWidth
                }
                NavBtn {
                    width: implicitWidth
                    text: "Map Ch. 1-2"
                }

                NavBtn {
                    width: implicitWidth
                    text: "Map Ch. 3-4"
                }

                NavBtn {
                    width: implicitWidth
                    text: "Map Ch. 5-6"
                }
            }

            ActionBtn {
                id: powerBtn
                text: "Quit"
                icon.source: "businesscontrols/icons/power_settings_new-24px.svg"
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
                target: ch1Reader
                state: ""
            }

            PropertyChanges {
                target: ch4Reader
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
                target: ch1Reader
                state: ""
            }

            PropertyChanges {
                target: ch2Reader
                state: ""
            }

            PropertyChanges {
                target: ch3Reader
                state: ""
            }

            PropertyChanges {
                target: ch4Reader
                state: ""
            }

            PropertyChanges {
                target: ch5Reader
                state: ""
            }

            PropertyChanges {
                target: ch6Reader
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
                target: ch1Reader
                state: ""
            }

            PropertyChanges {
                target: ch4Reader
                state: ""
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/

