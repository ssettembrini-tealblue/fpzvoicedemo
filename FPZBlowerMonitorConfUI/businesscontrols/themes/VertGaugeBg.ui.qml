import QtQuick 2.15
import FPZBlowerMonitorConf 1.0
import QtGraphicalEffects 1.0

Item {
    id: root
    implicitWidth: 32
    implicitHeight: 4
    Image {
        id: image
        visible: false
        fillMode: Image.TileVertically
        source: "images/vert-gauge_ind.svg"
        anchors.fill: parent
        verticalAlignment: Image.AlignBottom
    }
    ColorOverlay {
        id: overlay
        visible: true
        anchors.fill: parent
        source: image
        color: "white"
        opacity: 0.2
    }
}
