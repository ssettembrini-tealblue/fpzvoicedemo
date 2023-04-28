import QtQuick 2.15

Item {
    id: root
    property bool enabled: true
    implicitWidth: childrenRect.width
    implicitHeight: image.height * 1.66
    Image {
        id: image
        anchors.top: parent.top
        source: enabled ? "images/custom-triple-gauge-needle-round.png" : "images/custom-triple-gauge-needle-round-disabled.png"
        sourceSize.height: 48
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

