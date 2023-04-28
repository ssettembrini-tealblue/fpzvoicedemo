import QtQuick 2.15
import FPZBlowerMonitorConfUI 1.0

Item {
    id: root
    property bool down: false
    implicitWidth: 144
    implicitHeight: 80
    y: down ? 4 : 0
    clip: true
    Rectangle {
        id: lowerRect
        anchors.fill: parent
        color: down ? ThemeConstants.colorNavBtnBgDown : ThemeConstants.colorNavBtnBg
    }
    Rectangle {
        id: upperRect
        color: "white"
        opacity: 0.2
        transformOrigin: Item.BottomRight
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        rotation: Math.atan(
                      lowerRect.height / lowerRect.width) * (180 / Math.PI)
        height: parent.height
        width: Math.sqrt(Math.pow(parent.width, 2) + Math.pow(parent.height, 2))
    }
}
