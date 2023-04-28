import QtQuick 2.15
import QtGraphicalEffects 1.15
import FPZBlowerMonitorConf 1.0
import "themes"

Item {
    id: root
    property alias color: colorOverlay.color
    implicitWidth: 48
    implicitHeight: 48

    Image {
        id: image
        visible: false
        source: "icons/sync-black-18dp.svg"

        anchors.fill: parent
        sourceSize.height: height
        sourceSize.width: width
        fillMode: Image.PreserveAspectFit
    }

    states: [
        State {
            name: "alert"

            PropertyChanges {
                target: image
                source: "icons/sync_problem-24px.svg"
            }

            PropertyChanges {
                target: colorOverlay
                color: Constants.colorWarning
                opacity: 1
            }
        },
        State {
            name: "running"

            PropertyChanges {
                target: colorOverlay
                color: ThemeConstants.colorInfo
                opacity: 1
            }
        },
        State {
            name: "error"

            PropertyChanges {
                target: image
                source: "icons/sync_problem-24px.svg"
            }

            PropertyChanges {
                target: colorOverlay
                color: Constants.colorError
                opacity: 1
            }
        }
    ]
    ColorOverlay {
        id: colorOverlay
        color: ThemeConstants.colorStatusFg
        opacity: 0.5
        source: image
        anchors.fill: parent
    }
}
