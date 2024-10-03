import QtQuick 2.12
import QtGraphicalEffects 1.12
import FPZBlowerMonitorConf 1.0
import "themes"

Item {
    id: root
    property alias color: colorOverlay.color
    property bool detected: false
    implicitWidth: 48
    implicitHeight: 48

    Image {
        id: image
        visible: false
        source: "icons/mic-24px.svg"

        anchors.fill: parent
        sourceSize.height: height
        sourceSize.width: width
        fillMode: Image.PreserveAspectFit
    }
    ColorOverlay {
        id: colorOverlay
        source: image
        anchors.fill: parent
        color: ThemeConstants.colorStatusFg
        opacity: 0.5
    }

    states: [
        State {
            name: "detected"
            when: root.detected

            PropertyChanges {
                target: colorOverlay
                color: "#a0db8e"
                opacity: 1
            }
        },
        State {
            name: "not-detected"
            when: !root.detected & root.enabled
            PropertyChanges {
                target: colorOverlay
                color: Constants.colorError //"#ff6f69"
                opacity: 1
            }
        },
        State {
            name: "disabled"
            when: !root.detected && !root.enabled

            PropertyChanges {
                target: colorOverlay
                color: "grey"
                opacity: 1
            }
        }
    ]
}
