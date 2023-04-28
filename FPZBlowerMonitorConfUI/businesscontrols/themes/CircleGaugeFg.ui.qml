import QtQuick 2.15
import "../../businesscontrols"

Item {
    id: root
    property alias color: arc.strokeColor
    property alias begin: arc.begin
    property alias end: arc.end
    ArcItem {
        id: arc
        antialiasing: true
        dashPattern: [0.16, 0.16, 0, 0]
        strokeWidth: 16
        fillColor: "transparent"
        strokeStyle: 2
        anchors.fill: parent
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
