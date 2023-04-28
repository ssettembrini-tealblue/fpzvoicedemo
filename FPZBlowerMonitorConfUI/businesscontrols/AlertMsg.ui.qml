import QtQuick 2.15
import FPZBlowerMonitorConf 1.0
import "themes"

Rectangle {
    id: alertMsg
    width: 600
    height: 64
    color: ThemeConstants.colorInfo
    radius: 8
    property alias message: label.text
    property alias label: label

    Text {
        id: label

        color: ThemeConstants.colorMsgFg
        text: "<message line one.>\n<message line two.>"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
        lineHeight: 1.2
        opacity: 0.5
        font: Constants.fontMain
    }
    states: [
        State {
            name: "warning"

            PropertyChanges {
                target: alertMsg
                color: Constants.colorWarning
            }
        },
        State {
            name: "error"

            PropertyChanges {
                target: alertMsg
                color: Constants.colorError
            }
        },
        State {
            name: "success"

            PropertyChanges {
                target: alertMsg
                color: Constants.colorSuccess
            }
        }
    ]
}
