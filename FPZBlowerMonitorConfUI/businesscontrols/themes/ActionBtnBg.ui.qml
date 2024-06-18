import QtQuick 2.15
import QtGraphicalEffects 1.15
import FPZBlowerMonitorConf 1.0

Item {
    id: root
    property bool down: false
    property alias outerRect: outerRect
    implicitWidth: 144
    implicitHeight: 56
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: down ? 2 : 0 //4 : 0
    opacity: root.enabled ? 1 : 0.8
    DropShadow {
        anchors.fill: outerRect
        source: outerRect
        horizontalOffset: 0
        verticalOffset: root.enabled ? (root.down ? 2 : 6) : 2 //4 : 12
        radius: verticalOffset
        color: "#66000000"
    }
    DropShadow {
        anchors.fill: outerRect
        source: outerRect
        horizontalOffset: 0
        verticalOffset: root.enabled ? (root.down ? 0 : -1) : 0
        radius: root.down ? 4 : 8
        color: "#66000000"
    }
    Rectangle {
        id: outerRect
        // width: parent.width
        // height: parent.height
        anchors.fill: parent
        //anchors.rightMargin: 16
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
        //anchors.rightMargin: 20
        radius: 24
        gradient: Gradient {
            GradientStop {
                position: 0
                color: root.enabled ? Qt.lighter(
                                          ThemeConstants.colorPowerBtnBg,
                                          1.15) : "#EBEBE4"
            }
            GradientStop {
                position: 1
                color: root.enabled ? ThemeConstants.colorPowerBtnBg : "#EBEBE4"
            }
        }
    }
}
