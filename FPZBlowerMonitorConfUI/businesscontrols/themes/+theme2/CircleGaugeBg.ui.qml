import QtQuick 2.15
Item {
    id: root
    property alias begin: arc.begin
    property alias end: arc.end
    ArcItem {
        id: arc
        anchors.fill: parent
        strokeColor: ThemeConstants.colorGaugeBg
        antialiasing: true
        strokeWidth: 16
        fillColor: "transparent"
    }
}
