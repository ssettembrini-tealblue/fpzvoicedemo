import QtQuick 2.15
import FPZBlowerMonitorConf 1.0
import "themes"
import "../../FPZBlowerMonitorConfUI"

ParamReader {
    id: root
    property real minimumValue: 0
    property real maximumValue: 100
    height: 180
    Item {
        id: gauge
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 22
        width: 16
        VertGaugeBg {
            id: base
            anchors.fill: parent
        }
        VertGaugeFg {
            id: valueInd
            opacity: root.state == "" ? 0.2 : 1
            property var stateColors: {
                "": ThemeConstants.colorReaderFg,
                "error": Constants.colorError,
                "warning": Constants.colorWarning,
                "active": ThemeConstants.colorInfo
            }
            anchors.bottom: parent.bottom
            width: parent.width
            height: Math.max(
                        0,
                        (value - minimumValue) / (maximumValue - minimumValue) * parent.height)
            color: stateColors[root.state]
            Behavior on height {
                NumberAnimation{}
            }
        }
    }
    states: [
        State {
            name: "active"
            when: value && value < minWarningValue
            PropertyChanges {
                target: valueLabel
                opacity: 1
                color: ThemeConstants.colorInfo
            }

            PropertyChanges {
                target: unitLabel
                opacity: 1
            }
        },
        State {
            name: "warning"
            when: value >= minWarningValue && value <= minErrorValue
            PropertyChanges {
                target: valueLabel
                opacity: 1
                color: Constants.colorWarning
            }

            PropertyChanges {
                target: unitLabel
                opacity: 1
            }
        },
        State {
            name: "error"
            when: value >= minErrorValue
            PropertyChanges {
                target: valueLabel
                opacity: 1
                color: Constants.colorError
            }

            PropertyChanges {
                target: unitLabel
                opacity: 1
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;height:180;width:180}
}
##^##*/

