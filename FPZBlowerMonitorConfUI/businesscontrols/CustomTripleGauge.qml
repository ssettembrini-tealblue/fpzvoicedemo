import QtQuick 2.12
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Studio.Components 1.0
import QtQuick.Controls 2.15 as QQC2
import "themes"

Item {
    id: root
    readonly property alias paramLabel: paramLabel
    readonly property alias leftGauge: leftGauge
    readonly property alias bottomGauge: bottomGauge
    readonly property alias bottomGaugeParam: bottomGaugeParam
    readonly property alias bottomGaugeUnit: bottomGaugeUnit
    property int leftGaugeBegin: -130
    property int leftGaugeEnd: -15
    readonly property alias leftGaugeParam: leftGaugeParam
    readonly property alias leftGaugeUnit: leftGaugeUnit
    property int leftGaugeTickmarkStepSize: 10
    readonly property alias rightGauge: rightGauge
    property int rightGaugeBegin: 130
    property int rightGaugeEnd: 15
    readonly property alias rightGaugeParam: rightGaugeParam
    readonly property alias rightGaugeUnit: rightGaugeUnit
    property int rightGaugeTickmarkStepSize: 10
    property int labelInset
    property bool enabled: true
    property int labelBottomMargin: 24
    property bool showNumbers: false
    width: 180
    height: 180
    CustomTripleGaugeBg {
        enabled: root.enabled
        anchors.fill: parent
        visible: !root.useFlatBg
    }
    QQC2.Slider {
        id: bottomGauge
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 1/2.8
        leftPadding: 0
        topPadding: 0
        padding: 0
        y: - parent.height / 20
        background: null
        handle: CustomTripleGaugeNeedleLinear {
            enabled: root.enabled
            x: bottomGauge.leftPadding + bottomGauge.visualPosition * bottomGauge.availableWidth - width / 2
            y: bottomGauge.topPadding + bottomGauge.availableHeight / 2 - height * 6/7
            Behavior on x {
                NumberAnimation {}
            }
        }
    }
    CircularGauge {
        id: leftGauge
        anchors.fill: parent
        style: CircularGaugeStyle {
            id: leftGaugeStyle
            background: null
            foreground: null
            needle: CustomTripleGaugeNeedleRound {
                enabled: root.enabled
            }
            minimumValueAngle: root.leftGaugeBegin
            maximumValueAngle: root.leftGaugeEnd
            labelInset: root.labelInset
            tickmarkStepSize: root.leftGaugeTickmarkStepSize
            tickmark: if (!root.useFlatBg) null
            minorTickmark: if (!root.useFlatBg) null
            tickmarkLabel: Loader {
                property var styleInfo : styleData
                sourceComponent: tickmarkLabelComp
            }
        }
        Behavior on value {
            NumberAnimation {}
        }
    }
    CircularGauge {
        id: rightGauge
        anchors.fill: parent
        style: CircularGaugeStyle {
            id: rightGaugeStyle
            background: null
            foreground: CustomTripleGaugeFg {
                enabled: root.enabled
            }
            needle: CustomTripleGaugeNeedleRound {
                enabled: root.enabled
            }
            minimumValueAngle: root.rightGaugeBegin
            maximumValueAngle: root.rightGaugeEnd
            labelInset: root.labelInset
            tickmarkStepSize: root.rightGaugeTickmarkStepSize
            tickmark: if (!root.useFlatBg) null
            minorTickmark: if (!root.useFlatBg) null
            tickmarkLabel: Loader {
                property var styleInfo : styleData
                sourceComponent: tickmarkLabelComp
            }
        }
        Behavior on value {
            NumberAnimation {}
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
    Row {
        spacing: 8
        anchors.centerIn: parent
        Column {
            spacing:2
            Text {id: leftGaugeParam;color:ThemeConstants.colorCustomTripleGaugeFg;font.pixelSize: 8;anchors.right: parent.right}
            Text {id: leftGaugeUnit;color:ThemeConstants.colorCustomTripleGaugeFg;font.pixelSize: 8;anchors.right: parent.right}
            Text {text: Math.round(leftGauge.value);font.pixelSize: 8;color:ThemeConstants.colorCustomTripleGaugeFg;anchors.right: parent.right}
        }
        Column {
            spacing:2
            Text {id: rightGaugeParam;font.pixelSize: 8;color:ThemeConstants.colorCustomTripleGaugeFg}
            Text {id: rightGaugeUnit;font.pixelSize: 8;color:ThemeConstants.colorCustomTripleGaugeFg}
            Text {text: Math.round(rightGauge.value);font.pixelSize: 8;color:ThemeConstants.colorCustomTripleGaugeFg}
        }
    }
    Row {
        spacing:2
        Text {id: bottomGaugeParam;font.pixelSize: 8;color:ThemeConstants.colorCustomTripleGaugeFg}
        Text {id: bottomGaugeUnit;font.pixelSize: 8;color:ThemeConstants.colorCustomTripleGaugeFg}
        Text {text: Math.round(bottomGauge.value);font.pixelSize: 8;color:ThemeConstants.colorCustomTripleGaugeFg}
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 24
    }
    Component {
        id: imageBgComp
        CustomTripleGaugeBg {
            enabled: root.enabled
        }
    }
    Component {
        id: flatBgComp
        Rectangle {
            color: ThemeConstants.colorCustomRoundGaugeBgFlat
            radius: width / 2
        }
    }
    Component {
        id: tickmarkLabelComp
        Text {
            id: textItem
            text: styleInfo.value
            visible: root.showNumbers
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
