import QtQuick 2.15
import FPZBlowerMonitorConf 1.0
import "themes"

Item {
    id: statusArea
    readonly property alias connectedDeviceName: connectedDeviceName
    implicitWidth: 1280
    implicitHeight: 64
    StatusBarBg {
        id: statusAreaBg
        anchors.fill: parent
    }

    Text {
        id: connectedDeviceName
        color: "#ffffff"
        anchors.verticalCenter: parent.verticalCenter
        font: Constants.fontMainM
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
