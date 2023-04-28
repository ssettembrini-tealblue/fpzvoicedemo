import QtQuick 2.15

Item {
    id: root
    property alias begin: arc.begin
    property alias end: arc.end
    ArcItem {
        id: arc
        anchors.fill: parent
        strokeColor: ThemeConstants.colorReaderFg
        opacity: 0.2
        antialiasing: true
        dashPattern: [0.16, 0.16, 0, 0]
        strokeStyle: 2
        strokeWidth: 16
        fillColor: "transparent"
    }
}
