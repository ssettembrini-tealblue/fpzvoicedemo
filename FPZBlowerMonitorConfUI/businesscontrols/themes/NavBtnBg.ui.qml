import QtQuick 2.15
import FPZBlowerMonitorConf 1.0

Item {
    id: root
    property bool down: false
    implicitWidth: 144
    height: 80
    Rectangle {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        radius: 16
        gradient: Gradient {
            GradientStop {
                position: 0
                color: down ? Qt.darker(ThemeConstants.colorNavBtnBgDown,
                                        1.75) : Qt.lighter(
                                  ThemeConstants.colorNavBtnBg, 1.25)
            }
            GradientStop {
                position: 1
                color: down ? Qt.darker(ThemeConstants.colorNavBtnBgDown,
                                        1.5) : Qt.darker(
                                  ThemeConstants.colorNavBtnBg, 1.5)
            }
        }
    }
    Rectangle {
        anchors.fill: parent
        anchors.margins: 8
        anchors.leftMargin: 13
        anchors.rightMargin: 13
        radius: 12
        gradient: Gradient {
            GradientStop {
                position: 0
                color: down ? Qt.darker(ThemeConstants.colorNavBtnBgDown,
                                        1.25) : Qt.lighter(
                                  ThemeConstants.colorNavBtnBg, 1.75)
            }
            GradientStop {
                position: 1
                color: down ? ThemeConstants.colorInfo : ThemeConstants.colorNavBtnBg
            }
        }
    }
}



