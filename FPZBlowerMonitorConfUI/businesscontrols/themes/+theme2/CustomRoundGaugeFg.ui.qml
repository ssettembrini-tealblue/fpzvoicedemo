import QtQuick 2.15

Item {
    id: root
    property bool enabled: true
    Image {
        anchors.centerIn: parent
        sourceSize.width: 16
        source: root.enabled ? "+theme2/images/custom-round-gauge-fg.png" : "+theme2/images/custom-round-gauge-fg-disabled.png"
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

