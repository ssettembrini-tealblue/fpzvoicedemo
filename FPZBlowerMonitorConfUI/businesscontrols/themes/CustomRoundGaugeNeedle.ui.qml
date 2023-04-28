import QtQuick 2.15

Image {
    id: root
    property bool enabled: true
    source: enabled ? "images/custom-round-gauge-needle.png" : "images/custom-round-gauge-needle-disabled.png"
    sourceSize.height: 48
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

