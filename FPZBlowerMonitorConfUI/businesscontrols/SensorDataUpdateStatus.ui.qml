import QtQuick 2.15
import FPZBlowerMonitorConf 1.0
import "themes"

Row {
    id: sensorDataUpdateStatus
    property alias secs: secs
    spacing: 8
    SensorDataUpdateIndicator {
        id: sensorDataUpdateIndicator
    }

    Text {
        id: secs
        color: ThemeConstants.colorStatusFg
        text: qsTr("0")
        anchors.verticalCenter: parent.verticalCenter
        font: Constants.fontMain
        opacity: 0.5
    }

    Text {
        id: sensorDataLastUpdateLabel
        color: ThemeConstants.colorStatusFg
        text: qsTr("secs ago")
        anchors.verticalCenter: parent.verticalCenter
        font: Constants.fontMain
        opacity: 0.5
    }
    states: [
        State {
            name: "alert"

            PropertyChanges {
                target: sensorDataUpdateIndicator
                state: "alert"
            }

            PropertyChanges {
                target: secs
                color: Constants.colorWarning
                opacity: 1
            }

            PropertyChanges {
                target: sensorDataLastUpdateLabel
                color: Constants.colorWarning
                opacity: 1
            }
        },
        State {
            name: "running"

            PropertyChanges {
                target: sensorDataUpdateIndicator
                state: "running"
            }

            PropertyChanges {
                target: sensorDataLastUpdateLabel
                opacity: 1
            }

            PropertyChanges {
                target: secs
                opacity: 1
            }
        },
        State {
            name: "error"

            PropertyChanges {
                target: sensorDataUpdateIndicator
                state: "error"
            }

            PropertyChanges {
                target: sensorDataLastUpdateLabel
                color: Constants.colorError
                opacity: 1
            }

            PropertyChanges {
                target: secs
                color: Constants.colorError
                opacity: 1
            }
        }
    ]
}
