import QtQuick 2.12
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
import "themes"

CircularGauge {
    id: root
    readonly property alias paramLabel: paramLabel
    readonly property alias paramValue: paramValue
    property int labelInset
    property int begin: -135
    property int end: 135
    property int tickmarkStepSize: 100
    property bool enabled: true
    property bool showNumbers: true
    property bool showArc: false
    property bool showValue: false
    property int labelBottomMargin: 24
    property int valueBottomMargin: 44
    property int valueHCenterOffset: 0
    property bool useFlatBg: false
    property real arcSize: 1
    property int arcWidth: 8
    property color arcColor: "green"
    width: 180
    height: 180
    Behavior on value {
        NumberAnimation {}
    }
    style: CircularGaugeStyle {
        id: styleObj
        background: Item {
            CustomRoundGaugeBg {
                visible: !root.useFlatBg
                enabled: root.enabled
                anchors.fill: parent
            }
            Rectangle {
                visible: root.useFlatBg
                color: ThemeConstants.colorCustomRoundGaugeBgFlat
                radius: width / 2
                anchors.fill: parent
            }
            ArcItem {
                id: gaugeArc
                visible: showArc
                anchors.centerIn: parent
                width: root.arcSize * parent.width
                height: width
                antialiasing: true
                strokeWidth: root.arcWidth
                fillColor: "transparent"
                begin: root.begin
                end: Math.max(
                         root.begin,
                         root.begin + (root.end - root.begin)
                         / (maximumValue - minimumValue) * (value - minimumValue))
                strokeColor: root.arcColor
            }
        }
        needle: CustomRoundGaugeNeedle {
            enabled: root.enabled
        }
        foreground: CustomRoundGaugeFg {
            enabled: root.enabled
        }
        minimumValueAngle: root.begin
        maximumValueAngle: root.end
        labelInset: root.labelInset
        tickmarkStepSize: root.tickmarkStepSize
        tickmark: if (!root.useFlatBg) null
        minorTickmark: if (!root.useFlatBg) null
        tickmarkLabel: Loader {
            property var styleInfo : styleData
            sourceComponent: tickmarkLabelComp
        }
    }
    Text {
        id: paramLabel
        color: ThemeConstants.colorCustomRoundGaugeFg
        opacity: root.enabled ? 1 : 0.5
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 24
        anchors.bottomMargin: root.labelBottomMargin
    }
    Text {
        id: paramValue
        color: ThemeConstants.colorCustomRoundGaugeFg
        opacity: root.enabled ? 1 : 0.5
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: root.valueHCenterOffset
        anchors.bottomMargin: root.valueBottomMargin
        text: Math.round(root.value,2)
        visible: root.showValue
    }
    Component {
        id: tickmarkLabelComp
        Text {
            id: textItem
            text: styleInfo.value
            visible: root.showNumbers || root.useFlatBg
            color: ThemeConstants.colorCustomRoundGaugeFg
            font.pixelSize: 10
            antialiasing: true
        }
    }
}

/*##^##
Designer {
    D{i:0;height:160;width:160}
}
##^##*/
