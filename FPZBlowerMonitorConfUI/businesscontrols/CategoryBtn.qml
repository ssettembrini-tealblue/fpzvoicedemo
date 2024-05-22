import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import FPZBlowerMonitorConf 1.0
import "./themes"
Button {

    id: root

    width: 200
    height: 56
    enabled: true
    down: false
    text: qsTr("[catName]")
    leftPadding: 15
    rightPadding: 15
    display: icon.source==="" ? AbstractButton.TextBesideIcon : AbstractButton.TextOnly

    background:
        Item{
        anchors.verticalCenter: parent.verticalCenter
        opacity: root.enabled ? 1 : 0.8
        Rectangle {
            id: bg
            anchors.fill: parent
            anchors.margins: 4
            radius: 12
            color: root.enabled ? (root.down ? Qt.lighter(ThemeConstants.colorReaderBg, 1.55) :  Qt.lighter(ThemeConstants.colorReaderBg, 1.2)) : "#EBEBE4"

        }
    }
    contentItem:
        Item {
        implicitWidth: row.implicitWidth
        implicitHeight: row.implicitHeight

        opacity: root.enabled ? 1 : 0.5
        Row {
            id: row
            //anchors.verticalCenterOffset: root.down ? 4 : 0
            // anchors.horizontalCenterOffset: -8
            spacing: 16
            anchors.centerIn: parent
            Image {
                id: image
                visible: false
                source: root.icon.source
                width: root.icon.width
                height: root.icon.height
            }
            ColorOverlay {
                visible: root.display===AbstractButton.TextBesideIcon ? true : false
                source: image
                anchors.verticalCenter: row.verticalCenter
                width: image.width
                height: image.height
                color: root.enabled ? Constants.colorError : "grey"
            }
            Text {
                text: root.text
                font.capitalization: Font.AllUppercase
                font.family: Constants.fontMain.family
                font.weight: Constants.fontMain.weight
                font.pixelSize: Constants.fontMain.pixelSize
                color: "white"

            }
        }
    }
}
