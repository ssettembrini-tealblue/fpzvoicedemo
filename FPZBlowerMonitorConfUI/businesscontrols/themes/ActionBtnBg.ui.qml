import QtQuick 2.15
import QtGraphicalEffects 1.15
import FPZBlowerMonitorConf 1.0

Item {
    id: root
    property bool down: false
    implicitWidth: 144
    implicitHeight: 56
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: down ? 4 : 0
    DropShadow {
        anchors.fill: outerRect
        source: outerRect
        horizontalOffset: 0
        verticalOffset: root.down ? 4 : 12
        radius: verticalOffset
        color: "#66000000"
    }
    DropShadow {
        anchors.fill: outerRect
        source: outerRect
        horizontalOffset: 0
        verticalOffset: root.down ? 0 : -1
        radius: root.down ? 4 : 8
        color: "#66000000"
    }
    Rectangle {
        id: outerRect
        anchors.fill: parent
        anchors.rightMargin: 16
        radius: 24
        gradient: Gradient {
            GradientStop {
                position: 0
                color: ThemeConstants.colorPowerBtnBg
            }
            GradientStop {
                position: 1
                color: Qt.darker(ThemeConstants.colorPowerBtnBg, 1.5)
            }
        }
    }
    Rectangle {
        id: innerRect
        anchors.fill: parent
        anchors.margins: 4
        anchors.rightMargin: 20
        radius: 24
        gradient: Gradient {
            GradientStop {
                position: 0
                color: Qt.lighter(ThemeConstants.colorPowerBtnBg, 1.15)
            }
            GradientStop {
                position: 1
                color: ThemeConstants.colorPowerBtnBg
            }
        }
    }
}
