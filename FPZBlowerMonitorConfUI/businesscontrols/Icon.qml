import QtQuick 2.15
import QtGraphicalEffects 1.15

Item {
    id: root
    property url source
    property string name: "normal"
    property color color: "black"
    implicitWidth: 24
    implicitHeight: implicitWidth
    Image {
        id: image
        source: root.name ? ("./icons/" + root.name + ".svg") : root.source
        smooth: true
        visible: false
        width: root.width
        height: root.height
        sourceSize: Qt.size(root.width, root.height)
    }
    ColorOverlay {
        anchors.fill: image
        source: image
        color: root.color
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:4}
}
##^##*/
