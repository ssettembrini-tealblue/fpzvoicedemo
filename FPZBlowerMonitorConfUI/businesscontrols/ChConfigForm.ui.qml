import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "themes"

Pane {
    id: root
    property alias minErrorValue: minErrorValue.text
    property alias minWarningValue: minWarningValue.text
    property alias maxValue: maxValue.text
    property alias res: res.text
    property alias minValue: minValue.text
    property alias unit: unit.text
    property alias label: label.text
    property alias chNum: chNum.text
    property alias icon: icon.text
    property alias sensorType: sensorType.text
    property alias decimalsValue: decimalsValue.text

    background: ReaderBg {
        anchors.fill: parent
    }

    GridLayout {
        id: inputGrid
        rowSpacing: 16
        columnSpacing: 16
        columns: 2

        Text {
            id: chNum
            Layout.fillWidth: true
            text: "<Channel number>"
            font.pixelSize: 16
            Layout.columnSpan: 2
            color: ThemeConstants.colorReaderFg
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            spacing: 4
            RowLayout {
                spacing: 8
                Text {
                    text: qsTr("Label")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                    color: ThemeConstants.colorReaderFg
                }
                ChConfigField {
                    id: label
                    Layout.alignment: Qt.AlignVCenter
                    text: "<>"
                }
            }
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: qsTr("Unit")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                    color: ThemeConstants.colorReaderFg
                }
                ChConfigField {
                    id: unit
                    text: "<>"
                    font.pixelSize: 12
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: qsTr("Resolution")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                    color: ThemeConstants.colorReaderFg
                }
                ChConfigField {
                    id: res
                    text: "0"
                    font.pixelSize: 12
                }
            }

            RowLayout {
                Text {
                    color: ThemeConstants.colorReaderFg
                    text: qsTr("Icon")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                }

                ChConfigField {
                    id: icon
                    text: "home"
                    font.pixelSize: 12
                }
                spacing: 8
                Layout.fillWidth: true
            }

            RowLayout {
                Text {
                    color: ThemeConstants.colorReaderFg
                    text: qsTr("Sensor Type")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                }

                ChConfigField {
                    id: sensorType
                    text: "V"
                    font.pixelSize: 12
                }
                spacing: 8
                Layout.fillWidth: true
            }
        }

        ColumnLayout {
            id: columnLayout
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: qsTr("Error Value")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                    color: ThemeConstants.colorReaderFg
                }
                ChConfigField {
                    id: minErrorValue
                    text: "0"
                    font.pixelSize: 12
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: qsTr("Min Value")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                    color: ThemeConstants.colorReaderFg
                }
                ChConfigField {
                    id: minValue
                    text: "0"
                    font.pixelSize: 12
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: qsTr("Max Value")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                    color: ThemeConstants.colorReaderFg
                }
                ChConfigField {
                    id: maxValue
                    text: "0"
                    font.pixelSize: 12
                }
            }
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: qsTr("Warn Value")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                    color: ThemeConstants.colorReaderFg
                }
                ChConfigField {
                    id: minWarningValue
                    text: "0"
                    font.pixelSize: 12
                }
            }
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: qsTr("Decimals")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 56
                    color: ThemeConstants.colorReaderFg
                }
                ChConfigField {
                    id: decimalsValue
                    text: "2"
                    font.pixelSize: 12
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;height:235;width:360}
}
##^##*/

