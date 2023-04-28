import QtQuick 2.15
import QtGraphicalEffects 1.0
import FPZBlowerMonitorConf 1.0
import "themes"

Item {
    id: root
    width: 40
    height: 180
    property alias paramIcon: paramIcon
    property alias paramLabel: paramLabel
    ReaderBg {
        anchors.fill: parent
    }

    Image {
        id: paramIcon
        source: "icons/insert_emoticon-24px.svg"
        fillMode: Image.PreserveAspectFit
        width: 24
        height: 24
        sourceSize.width: width
        sourceSize.height: height
        visible: false
    }
    ColorOverlay {
        id: colorOverlay
        source: paramIcon
        anchors.topMargin: 22
        anchors.horizontalCenter: parent.horizontalCenter
        width: paramIcon.width
        height: paramIcon.height
        color: ThemeConstants.colorReaderFg
        anchors.top: parent.top
        anchors.margins: 12
    }
    Text {
        id: paramLabel
        color: ThemeConstants.colorReaderFg
        text: qsTr("[param]")
        anchors.bottom: parent.bottom
        anchors.margins: 12
        font.family: Constants.fontMain.family
        font.weight: Constants.fontMain.weight
        font.pixelSize: Constants.fontMain.pixelSize
        anchors.bottomMargin: 34
        anchors.horizontalCenter: parent.horizontalCenter
        rotation: -90
        transformOrigin: Item.Center
    }
}



