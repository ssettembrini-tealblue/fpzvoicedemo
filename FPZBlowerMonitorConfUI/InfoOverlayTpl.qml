import QtQuick 2.12
import FPZBlowerMonitorConf 1.0
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.15
import "businesscontrols"
import "businesscontrols/themes"

Rectangle {
    id: root
    width: 500
    height: 440
    radius: 16
    color: ThemeConstants.colorMsgFg//colorReaderBg//"#A6ACAF"
    clip: true
    Image {
        id: img
        visible: false
        anchors.fill: parent
        source: "businesscontrols/themes/images/templatebg-pattern.png"
        fillMode: Image.Tile
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: root

        }

    }
    property int infoIndex:0
    property string lang: "IT"
    property string startInput: root.lang=="IT" ? DataConstants.startInputIt : DataConstants.startInputEn
    property string stopInput: root.lang=="IT" ? DataConstants.stopInputIt : DataConstants.stopInputEn
    property string increaseInput: root.lang=="IT" ? DataConstants.increaseInputIt : DataConstants.increaseInputEn
    property string decreaseInput: root.lang=="IT" ? DataConstants.decreaseInputIt : DataConstants.decreaseInputEn
    property string setInput: root.lang=="IT" ? DataConstants.setInputIt : DataConstants.setInputEn

    ColumnLayout{
        anchors.fill: parent
        spacing: 0
        Rectangle{
            color: ThemeConstants.colorInfo//ReaderBg
            Layout.preferredHeight: 64
            Layout.fillWidth: true
            radius: root.radius
            Rectangle{
                width: parent.width
                height: parent.height/2
                anchors.right: parent.right
                anchors.left:parent.left
                anchors.bottom: parent.bottom
                color: parent.color
            }
            Text{
                anchors.centerIn: parent
                text: root.lang=="IT" ? "Info Comandi Vocali"  : "Info Voice Commands"
                font.family: Constants.fontMainM.family
                font.weight: Constants.fontMainM.weight
                font.pixelSize: Constants.fontMainM.pixelSize
                color: ThemeConstants.colorReaderFg
            }
        }
        Rectangle{
            color:ThemeConstants.colorInfo// "black"//ThemeConstants.colorReaderBg
            Layout.preferredHeight: 2//4
            Layout.fillWidth: true
        }
        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true



            Rectangle{
                id: btnArea
                Layout.preferredWidth: colBtn.width+16
                Layout.fillHeight: true
                color: ThemeConstants.colorReaderBg
                radius: root.radius
                Rectangle{
                    width: parent.width
                    height: parent.height/2
                    anchors.right: parent.right
                    anchors.top:parent.top
                    anchors.left: parent.left
                    color: parent.color
                }
                Rectangle{
                    width: parent.width/2
                    height: parent.height
                    anchors.right: parent.right
                    anchors.top:parent.top
                    anchors.bottom: parent.bottom
                    color: parent.color
                }
                ColumnLayout{
                    id: colBtn
                    spacing: 12//16
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenterOffset: 8
                    anchors.horizontalCenter: parent.horizontalCenter
                    //width: childrenRect.width
                    // anchors.left: parent.left
                    // anchors.right: parent.right
                    // anchors.rightMargin: 24//16
                    // anchors.leftMargin: 24//16
                    ActionBtn {
                        id: showStartInfoBtn
                        Layout.alignment: Qt.AlignHCenter
                        text: "Start"
                        Layout.preferredWidth: 130
                        Layout.preferredHeight:56
                        onClicked: root.infoIndex = 0
                        down: root.infoIndex == 0

                    }
                    ActionBtn {
                        id: showStopInfoBtn
                        text: "Stop"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 130
                        Layout.preferredHeight:56
                        onClicked: root.infoIndex = 1
                        down: root.infoIndex == 1

                    }

                    ActionBtn {
                        id: showIncreaseInfoBtn
                        text: "Increase"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 130
                        Layout.preferredHeight:56
                        onClicked: root.infoIndex = 2
                        down: root.infoIndex == 2
                    }
                    ActionBtn {
                        id: showDecreasetInfoBtn
                        Layout.alignment: Qt.AlignHCenter
                        text: "Decrease"
                        Layout.preferredWidth: 130
                        Layout.preferredHeight:56
                        onClicked: root.infoIndex = 3
                        down: root.infoIndex == 3

                    }
                    ActionBtn {
                        id: showSetInfoBtn
                        Layout.alignment: Qt.AlignHCenter
                        text: "Set"
                        Layout.preferredWidth: 130
                        Layout.preferredHeight:56
                        onClicked: root.infoIndex = 4
                        down: root.infoIndex == 4
                    }

                }
            }

            // Rectangle{
            //     color: "black"//ThemeConstants.colorReaderBg
            //     Layout.preferredWidth: 4
            //     Layout.fillHeight: true
            // }
            Rectangle{
                id: contentArea
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "transparent"//ThemeConstants.colorPowerBtnBg
                clip: true
                Flickable{
                    anchors.top: parent.top
                    anchors.topMargin: 32
                    anchors.left: parent.left
                    anchors.leftMargin: 32
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 32
                    width: parent.width //Measures.module * 80
                    flickableDirection: Flickable.VerticalFlick

                    contentWidth: width
                    contentHeight: txtArea.height

                    ScrollBar.vertical: ScrollBar {
                        width: 10
                        contentItem:  Rectangle{
                            radius: 4
                            color: ThemeConstants.colorReaderBg
                        }

                        anchors.right: parent.right
                        //anchors.rightMargin: Measures.module * 4
                        policy: ScrollBar.AsNeeded
                    }
                    TextArea{
                        id: txtArea
                        font.family: Constants.fontMainS.family
                        font.weight: Constants.fontMainS.weight
                        font.pixelSize: Constants.fontMainS.pixelSize
                        //color: ThemeConstants.colorReaderFg
                        //anchors.fill: parent
                        //horizontalAlignment: Text.AlignHCenter
                        height: implicitHeight
                        readOnly: true
                        anchors.top: parent.top
                        anchors.left: parent.left
                        //anchors.horizontalCenter: parent.horizontalCenter
                        text: root.infoIndex == 0 ? root.startInput : (root.infoIndex == 1 ? root.stopInput : (root.infoIndex == 2 ? root.increaseInput :(root.infoIndex == 3 ? root.decreaseInput : root.setInput)))
                    }
                }
            }

            // StackLayout {
            //     id: contentArea
            //     currentIndex: root.infoIndex
            //     Layout.fillHeight: true
            //     Layout.fillWidth: true

            //     Rectangle{
            //         Layout.fillHeight: true
            //         Layout.fillWidth: true
            //         color: "red"

            //     }
            //     Rectangle{
            //         Layout.fillHeight: true
            //         Layout.fillWidth: true
            //         color: "blue"

            //     }
            //     Rectangle{
            //         Layout.fillHeight: true
            //         Layout.fillWidth: true
            //         color: "green"

            //     }
            //     Rectangle{
            //         Layout.fillHeight: true
            //         Layout.fillWidth: true
            //         color: "yellow"

            //     }

            //     Rectangle{
            //         Layout.fillHeight: true
            //         Layout.fillWidth: true
            //         color: "orange"

            //     }



            // }
        }
    }}
