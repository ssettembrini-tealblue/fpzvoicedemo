import QtQuick 2.15

Item {
    id: root
    property bool enabled: true
    Image {
        anchors.centerIn: parent
        source: root.enabled ? "images/custom-round-gauge-fg.png" : "images/custom-round-gauge-fg-disabled.png"
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

