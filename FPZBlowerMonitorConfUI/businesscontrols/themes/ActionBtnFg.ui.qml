import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import FPZBlowerMonitorConf 1.0

Item {
    id: root
    property alias image: image
    property bool down: false
    property Button control
    implicitWidth: row.implicitWidth
    implicitHeight: row.implicitHeight

    opacity: root.enabled ? 1 : 0.5
    Row {
        id: row
        anchors.verticalCenterOffset: down ? 4 : 0
        //anchors.horizontalCenterOffset: -8
        spacing: 16
        anchors.centerIn: parent
        Image {
            id: image
            visible: false
            source: control.icon.source
            width: control.icon.width
            height: control.icon.height
        }
        ColorOverlay {
            visible: control.display === AbstractButton.TextBesideIcon ? true : false
            source: image
            anchors.verticalCenter: row.verticalCenter
            width: image.width
            height: image.height
            color: root.enabled ? Constants.colorError : "grey"
        }
        Text {
            text: control.text
            font.capitalization: Font.AllUppercase
            font.family: Constants.fontMain.family
            font.weight: Constants.fontMain.weight
            font.pixelSize: Constants.fontMain.pixelSize
        }
    }
}
