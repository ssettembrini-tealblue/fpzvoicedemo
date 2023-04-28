import QtQuick 2.15

Image {
    id: root
    property bool enabled: true
    source: enabled ? "images/custom-triple-gauge-bg.png" : "images/custom-triple-gauge-bg-disabled.png"
}
