import QtQuick 2.15
import "../../../businesscontrols"

Item {
    id: root
    property alias color: arc.strokeColor
    property alias begin: arc.begin
    property alias end: arc.end
    ArcItem {
        id: arc
        antialiasing: true
        strokeWidth: 16
        fillColor: "transparent"
        anchors.fill: parent
    }
}
