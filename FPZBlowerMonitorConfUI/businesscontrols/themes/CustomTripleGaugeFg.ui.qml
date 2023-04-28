import QtQuick 2.15

Item {
    id: root
    property bool enabled: true
    implicitHeight: 90
    implicitWidth: 90
    Image {
        anchors.centerIn: parent
        width: parent.width * 6 / 11
        height: parent.height * 6 / 11
        source: enabled ? "images/custom-triple-gauge-fg.png" : "images/custom-triple-gauge-fg-disabled.png"
    }
}
