import QtQuick 2.15
import QtGraphicalEffects 1.0
import FPZBlowerMonitorConf 1.0
import "themes"

Item {
    id: root
    width: 180
    height: 180
    property real value
    property real minWarningValue: 30
    property real minErrorValue: 45
    property real minimumValue: 0
    property real maximumValue: 100
    property alias paramIcon: paramIcon
    property alias paramLabel: paramLabel
    property alias unitLabel: unitLabel

    ReaderBg {
        id: readerBg
        anchors.fill: parent
    }

    Item {
        id: contentArea
        width: 136
        height: 136
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Image {
            id: paramIcon
            source: "icons/insert_emoticon-24px.svg"
            anchors.topMargin: 0
            fillMode: Image.PreserveAspectFit
            width: 24
            height: 24
            anchors.right: parent.right
            anchors.top: parent.top
            sourceSize.width: width
            sourceSize.height: height
            visible: false
        }

        ColorOverlay {
            id: colorOverlay
            x: 112
            y: 0
            source: paramIcon
            anchors.right: parent.right
            width: paramIcon.width
            height: paramIcon.height
            color: ThemeConstants.colorReaderFg
        }

        Column {
            id: valueGroup
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Text {
                id: paramLabel
                color: ThemeConstants.colorReaderFg
                text: qsTr("[param]")
                font.family: Constants.fontMainS.family
                font.weight: Constants.fontMainS.weight
                font.pixelSize: Constants.fontMainS.pixelSize
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: valueLabel
                opacity: 0.5
                color: ThemeConstants.colorReaderFg
                text: root.value // || "NaN"
                font.family: Constants.fontMonoL.family
                font.weight: Constants.fontMonoL.weight
                font.pixelSize: Constants.fontMonoL.pixelSize
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: unitLabel
                opacity: 0.5
                color: ThemeConstants.colorReaderFg
                text: qsTr("[unit]")
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: Constants.fontMainL.family
                font.weight: Constants.fontMainL.weight
                font.pixelSize: Constants.fontMainL.pixelSize
            }
        }

        CircleGaugeBg {
            id: gaugeBase
            anchors.fill: parent
            begin: -135
            end: 135
        }

        CircleGaugeFg {
            id: gaugeValue
            opacity: root.state == "" ? 0.2 : 1
            property var stateColors: ({
                                           "error": Constants.colorError,
                                           "warning": Constants.colorWarning,
                                           "active": ThemeConstants.colorInfo,
                                           "": ThemeConstants.colorReaderFg
                                       })
            anchors.fill: parent
            begin: gaugeBase.begin
            end: Math.max(
                     gaugeBase.begin,
                     gaugeBase.begin + (gaugeBase.end - gaugeBase.begin)
                     / (maximumValue - minimumValue) * (value - minimumValue))
            color: stateColors[root.state]
            Behavior on end {
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
        },
        State {
            name: "disconnected"
            PropertyChanges {
                target: readerBg
                color: "grey"
                opacity: 0.9
            }
        }
    ]
}



