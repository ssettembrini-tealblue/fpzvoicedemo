import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import FPZBlowerMonitorConf 1.0

Item {
    property bool down: false
    property Button control
    implicitWidth: row.implicitWidth
    implicitHeight: row.implicitHeight
    Row {
        id: row
        spacing: 16
        anchors.centerIn: parent
        anchors.verticalCenterOffset: down ? 4 : 0
        anchors.horizontalCenterOffset: -8
        Image {
            id: image
            visible: false
            source: control.icon.source
            width: control.icon.width
            height: control.icon.height
        }
        ColorOverlay {
			visible: control.display===AbstractButton.TextBesideIcon ? true : false
            source: image
            anchors.verticalCenter: row.verticalCenter
            width: image.width
            height: image.height
            color: ThemeConstants.colorPowerBtnFg
        }
        Text {
            text: control.text
            font.capitalization: Font.AllUppercase
            font.family: Constants.fontMain.family
            font.weight: Constants.fontMain.weight
            font.pixelSize: Constants.fontMain.pixelSize
            color: ThemeConstants.colorPowerBtnFg
			leftPadding: 15
            rightPadding: 15
        }
    }
}
