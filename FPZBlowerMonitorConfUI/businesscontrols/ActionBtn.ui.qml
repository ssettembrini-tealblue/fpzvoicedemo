import QtQuick 2.15
import QtQuick.Controls 2.15
import FPZBlowerMonitorConf 1.0
import "themes"

Button {
    id: root
    //leftPadding: (width - contentItem.implicitWidth) / 2
    //rightPadding: leftPadding
    //topPadding: (height - contentItem.implicitHeight) / 2
    //bottomPadding: topPadding
    text: qsTr("Off")
    leftPadding: 15
    rightPadding: 15
    display: icon.source==="" ? AbstractButton.TextBesideIcon : AbstractButton.TextOnly
    //icon.source: "icons/power_settings_new-24px.svg"
    background: ActionBtnBg {
        down: root.down
    }
    contentItem: ActionBtnFg {
        control: root
        down: root.down
    }
}
