import QtQuick 2.15
import QtQuick.Controls 2.15
import FPZBlowerMonitorConf 1.0
import "themes"

TabButton {
    id: root
    text: ""//qsTr("Label")
    implicitHeight: 58
    implicitWidth: 70
    background:
        Rectangle {
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            radius: 16
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: root.checked ? Qt.darker(ThemeConstants.colorNavBtnBgDown,
                                            1.75) : Qt.lighter(
                                      ThemeConstants.colorNavBtnBg, 1.25)
                }
                GradientStop {
                    position: 1
                    color: root.checked ? Qt.darker(ThemeConstants.colorNavBtnBgDown,
                                            1.5) : Qt.darker(
                                      ThemeConstants.colorNavBtnBg, 1.5)
                }
            }
        }
        Rectangle {
            anchors.fill: parent
            anchors.margins:5
            anchors.leftMargin: 9//13
            anchors.rightMargin: 9//13
            radius: 12
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: root.checked ? Qt.darker(ThemeConstants.colorNavBtnBgDown,
                                            1.25) : Qt.lighter(
                                      ThemeConstants.colorNavBtnBg, 1.75)
                }
                GradientStop {
                    position: 1
                    color: root.checked ? ThemeConstants.colorInfo : ThemeConstants.colorNavBtnBg
                }
            }
        }

    contentItem:Item{
        anchors.fill: parent
        Icon{
            name: "info-circle"
            height: 42
            width: 42
            color: root.checked ? ThemeConstants.colorNavBtnBg : ThemeConstants.colorInfo
            anchors.centerIn: parent
        }
    }
    //     Text {
    //     id: label
    //     text: root.text
    //     horizontalAlignment: Text.AlignHCenter
    //     verticalAlignment: Text.AlignVCenter
    //     elide: Text.ElideRight
    //     color: root.checked ? ThemeConstants.colorNavBtnFgDown : ThemeConstants.colorNavBtnFg
    //     font.family: Constants.fontMain.family
    //     font.weight: Constants.fontMain.weight
    //     font.pixelSize: Constants.fontMain.pixelSize
    //     font.capitalization: Font.AllUppercase
    // }
}
