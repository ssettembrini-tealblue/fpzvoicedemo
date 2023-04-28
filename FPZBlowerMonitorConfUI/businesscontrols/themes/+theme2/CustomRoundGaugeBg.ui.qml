import QtQuick 2.15

Image {
    id: root
    property bool enabled: true
    source: enabled ? "+theme2/images/custom-round-gauge-bg.png" : "+theme2/images/custom-round-gauge-bg-disabled.png"
}
