import QtQuick 2.12
import QtGraphicalEffects 1.12
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
        source: "icons/modbus-24px.svg"

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
            name: "searching"

            PropertyChanges {
                target: image
                source: "icons/modbus-24px.svg"
            }

            PropertyChanges {
                target: colorOverlay
                color: Constants.colorWarning
                opacity: 1
            }
        },
        State {
            name: "connected"
            PropertyChanges {
                target: image
                source: "icons/modbus_connected-24px.svg"
            }

            PropertyChanges {
                target: colorOverlay
                color: ThemeConstants.colorInfo
                opacity: 1
            }
        },
        State {
            name: "disabled"
            PropertyChanges {
                target: image
                source: "icons/modbus_disabled-24px.svg"
            }

            PropertyChanges {
                target: colorOverlay
                color: Constants.colorError
                opacity: 1
            }
        }
    ]
}
