import QtQuick 2.15

Image {
    id: root
    property bool enabled: true
    source: enabled ? "images/custom-round-gauge-bg.png" : "images/custom-round-gauge-bg-disabled.png"
}
