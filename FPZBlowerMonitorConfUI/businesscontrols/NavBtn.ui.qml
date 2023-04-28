import QtQuick 2.15
import QtQuick.Controls 2.15
import FPZBlowerMonitorConf 1.0
import "themes"

TabButton {
    id: root
    text: qsTr("Label")

    background: NavBtnBg {
        id: navBtnBg
        anchors.fill: root
        down: root.checked
    }

    contentItem: Text {
        id: label
        text: root.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: root.checked ? ThemeConstants.colorNavBtnFgDown : ThemeConstants.colorNavBtnFg
        font.family: Constants.fontMain.family
        font.weight: Constants.fontMain.weight
        font.pixelSize: Constants.fontMain.pixelSize
        font.capitalization: Font.AllUppercase
    }
}



