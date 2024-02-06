import QtQuick 2.15
import QtGraphicalEffects 1.0
import FPZBlowerMonitorConf 1.0
import "themes"

Item {
    id: root
    width: 180
    height: 180
    property real value
    property real minWarningValue
    property real minErrorValue
    property alias contentArea: contentArea
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

        Column {
            id: paramGroup
            anchors.right: parent.right
            spacing: 0

            Image {
                id: paramIcon
                source: "icons/insert_emoticon-24px.svg"
                fillMode: Image.PreserveAspectFit
                width: 24
                height: 24
                anchors.right: parent.right
                sourceSize.width: width
                sourceSize.height: height
                visible: false
            }
            ColorOverlay {
                id: colorOverlay
                source: paramIcon
                anchors.right: parent.right
                width: paramIcon.width
                height: paramIcon.height
                color: ThemeConstants.colorReaderFg
            }
            Text {
                id: paramLabel
                height: 24
                color: ThemeConstants.colorReaderFg
                text: qsTr("[param]")
                font.family: Constants.fontMainS.family
                font.weight: Constants.fontMainS.weight
                font.pixelSize: Constants.fontMainS.pixelSize
            }
        }

        Column {
            id: valueGroup
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Text {
                id: valueLabel
                opacity: 0.5
                color: ThemeConstants.colorReaderFg
                text: root.value || "NaN"
                anchors.right: parent.right
                font.family: Constants.fontMonoL.family
                font.weight: Constants.fontMonoL.weight
                font.pixelSize: Constants.fontMonoL.pixelSize
            }

            Text {
                id: unitLabel
                opacity: 0.5
                color: ThemeConstants.colorReaderFg
                text: qsTr("[unit]")
                anchors.right: parent.right
                font.family: Constants.fontMainL.family
                font.weight: Constants.fontMainL.weight
                font.pixelSize: Constants.fontMainL.pixelSize
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
            when: value >= minWarningValue && value < minErrorValue
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
