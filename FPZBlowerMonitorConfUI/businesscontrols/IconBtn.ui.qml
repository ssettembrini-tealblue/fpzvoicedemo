import QtQuick 2.15
import QtQuick.Controls 2.15

AbstractButton {
    id: control
    property string name
    property color color
    icon.name: name
    icon.color: color
    implicitWidth: 24
    implicitHeight: 24
    hoverEnabled: true
    contentItem: Icon {
        opacity: control.enabled ? (control.hovered ? 0.8 : 1) : 0.5
        name: icon.name
        color: icon.color
        width: control.width
        height: width
    }
    background: Item {}
}

/*##^##
Designer {
    D{i:0;height:24;width:24}
}
##^##*/

