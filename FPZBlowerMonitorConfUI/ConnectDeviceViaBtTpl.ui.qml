import QtQuick 2.12
import FPZBlowerMonitorConf 1.0
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.12
import "businesscontrols"
import "businesscontrols/themes"

Rectangle {
    id: root
    width: Constants.width
    height: Constants.height

    color: Constants.backgroundColor
    property alias discoveredDeviceName: discoveredDeviceName
    state: "init"
    property alias stopDeviceDiscoveryBtn: stopDeviceDiscoveryBtn
    property alias knownDevicesListView: knownDevicesListView
    property alias newDevicesListView: newDevicesListView
    property alias connectKnownDeviceBtn: connectKnownDeviceBtn
    property alias connectNewDeviceBtn: connectNewDeviceBtn
    property string currentDeviceName
    property alias discoverAvailableDevicesBtn: discoverAvailableDevicesBtn
    property alias outcomeMsg: outcomeMsg
    property alias progressMsg: progressMsg
    property alias askBtPermissionsBtn: askBtPermissionsBtn
    property alias activateBtBtn: activateBtBtn
    property alias pairDeviceBtn: pairDeviceBtn
    property alias connectDeviceBtn: connectDeviceBtn
    signal newDeviceItemClicked(string deviceName)
    signal knownDeviceItemClicked(string deviceName)
    TemplateBg {
        id: templateBg
        anchors.fill: parent
    }
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.rightMargin: 48
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        anchors.topMargin: 48

        Item {
            id: mainContent
            Layout.fillHeight: true
            Layout.fillWidth: true

            Frame {
                id: newDevicesListFrame
                width: 640
                height: 480
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                ColumnLayout {
                    id: columnLayout1
                    anchors.fill: parent

                    AlertMsg {

                        label.font.capitalization: Font.AllUppercase
                        label.font.family: Constants.fontMain.family
                        label.font.weight: Constants.fontMain.weight
                        id: newDevicesLabel
                        message: qsTr("Select the device you want to connect:")
                        label.font.pixelSize: 16
                        label.horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                        label.font.bold: true
                    }

                    ListView {
                        id: newDevicesListView
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        delegate: ItemDelegate {
                            id: deviceDelegate
                            width: newDevicesListView.width
                            height: 48
                            text: modelData["deviceName"]
                            font.capitalization: Font.AllUppercase
                            font.family: Constants.fontMain.family
                            font.weight: Constants.fontMain.weight
                            font.pixelSize: Constants.fontMain.pixelSize

                            Connections {
                                target: deviceDelegate
                                onClicked: newDeviceItemClicked(
                                               modelData["deviceName"])
                            }
                        }
                        //model: availableDevicesModel
                    }
                }
            }

            Frame {
                id: knownDevicesListFrame
                width: 640
                height: 480
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                ColumnLayout {
                    id: columnLayout2
                    anchors.fill: parent
                    AlertMsg {
                        id: knownDevicesLabel
                        message: qsTr("Select the known device you want to connect:")
                        label.font.pixelSize: 16
                        label.horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                        label.font.bold: true
                    }

                    ListView {
                        id: knownDevicesListView
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        delegate: ItemDelegate {
                            id: deviceDelegate1
                            width: knownDevicesListView.width
                            height: 48
                            text: modelData["deviceName"]
                            Connections {
                                target: deviceDelegate1
                                onClicked: knownDeviceItemClicked(
                                               modelData["deviceName"])
                            }
                        }
                        model: savedDevicesModel
                    }
                }
            }

            Column {
                id: outcomeMsgArea
                x: 472
                y: 269
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 32
                AlertMsg {
                    id: outcomeMsg
                    width: 240
                    height: 120
                    message: qsTr("<outcome message>")
                    label.font.capitalization: Font.AllUppercase
                    label.font.family: Constants.fontMain.family
                    label.font.weight: Constants.fontMain.weight

                    label.font.pixelSize: 16
                    label.horizontalAlignment: Text.AlignHCenter
                    label.verticalAlignment: Text.AlignVCenter
                    label.wrapMode: Text.WordWrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    label.lineHeight: 1.25
                }

                Column {
                    spacing: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    ActionBtn {
                        id: connectDeviceBtn
                        text: qsTr("Retry connecting device")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    ActionBtn {
                        id: pairDeviceBtn

                        text: qsTr("Retry pairing device")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    ActionBtn {
                        id: discoverAvailableDevicesBtn
                        text: qsTr("Retry discovering devices")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    ActionBtn {
                        id: activateBtBtn
                        text: qsTr("Retry activating Bluetooth")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    ActionBtn {
                        id: askBtPermissionsBtn
                        text: qsTr("Retry acquiring necessary permissions")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            Column {
                id: progressMsgArea
                anchors.verticalCenter: parent.verticalCenter
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 32
                    BusyIndicator {
                        id: busyIndicator
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Item {
                        id: labelArea
                        width: 240
                        height: 120

                        AlertMsg {
                            id: progressMsg
                            message: qsTr("<progress message>")
                            anchors.fill: parent
                            label.font.pixelSize: 16
                            label.horizontalAlignment: Text.AlignHCenter
                            label.verticalAlignment: Text.AlignVCenter
                            label.wrapMode: Text.WordWrap
                            label.lineHeight: 1.25
                        }
                    }
                }
                AlertMsg {

                    id: discoveredDeviceName
                    message: qsTr("<Discovered device name>")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                ActionBtn {
                    id: stopDeviceDiscoveryBtn
                    text: qsTr("Stop device discovery")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Frame {
                id: connectionPolicyFrame
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                Column {
                    id: column
                    spacing: 32
                    anchors.horizontalCenter: parent.horizontalCenter

                    AlertMsg {
                        id: label1
                        message: qsTr("What device would you like to connect?")
                        label.font.bold: true
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Button {
                        id: connectKnownDeviceBtn
                        text: qsTr("Existing device")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Button {
                        id: connectNewDeviceBtn
                        text: qsTr("New device")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "init"

            PropertyChanges {
                target: columnLayout
                visible: false
            }
        },
        State {
            name: "activatingBt"

            PropertyChanges {
                target: progressMsgArea
                visible: true
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: false
            }

            PropertyChanges {
                target: progressMsg
                message: qsTr("Checking Bluetooth is active...")
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }

            PropertyChanges {
                target: discoveredDeviceName
                visible: false
            }
        },
        State {
            name: "discoveringAvailableDevices"

            PropertyChanges {
                target: progressMsgArea
                visible: true
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: false
            }

            PropertyChanges {
                target: progressMsg
                message: qsTr("Discovering available devices...")
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: true
            }

            PropertyChanges {
                target: connectDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: pairDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: discoverAvailableDevicesBtn
                visible: false
            }

            PropertyChanges {
                target: activateBtBtn
                visible: false
            }

            PropertyChanges {
                target: askBtPermissionsBtn
                visible: false
            }
        },
        State {
            name: "checkingForKnownDevices"

            PropertyChanges {
                target: progressMsgArea
                visible: true
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: false
            }

            PropertyChanges {
                target: progressMsg
                message: qsTr("Checking for known devices...")
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }

            PropertyChanges {
                target: discoveredDeviceName
                visible: false
            }
        },
        State {
            name: "connectingDevice"

            PropertyChanges {
                target: progressMsgArea
                visible: true
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: false
            }

            PropertyChanges {
                target: progressMsg
                message: qsTr("Connecting device ") + currentDeviceName
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }

            PropertyChanges {
                target: discoveredDeviceName
                visible: false
            }
        },
        State {
            name: "deviceConnected"
            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: true
            }

            PropertyChanges {
                target: progressMsg
                visible: false
            }

            PropertyChanges {
                target: connectDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: discoverAvailableDevicesBtn
                visible: false
            }

            PropertyChanges {
                target: outcomeMsg
                visible: true
                color: "#0000ff"
                message: qsTr(
                             "Device ") + currentDeviceName + (" was connected")
            }

            PropertyChanges {
                target: pairDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: activateBtBtn
                visible: false
            }

            PropertyChanges {
                target: askBtPermissionsBtn
                visible: false
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }
        },
        State {
            name: "askingBtPermissions"
            PropertyChanges {
                target: progressMsgArea
                visible: true
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: false
            }

            PropertyChanges {
                target: progressMsg
                message: qsTr("Awaiting the necessary permissions to be granted...")
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }

            PropertyChanges {
                target: discoveredDeviceName
                visible: false
            }
        },
        State {
            name: "selectingNewDeviceToConnect"

            PropertyChanges {
                target: outcomeMsgArea
                visible: false
            }

            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: true
            }
        },
        State {
            name: "deviceCantConnect"
            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: true
            }

            PropertyChanges {
                target: progressMsg
                visible: false
            }

            PropertyChanges {
                target: connectDeviceBtn
                visible: true
            }

            PropertyChanges {
                target: discoverAvailableDevicesBtn
                visible: true
            }

            PropertyChanges {
                target: outcomeMsg
                color: "#be7502"
                message: qsTr("Device ") + currentDeviceName + (" couldn't connect.")
            }

            PropertyChanges {
                target: pairDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: activateBtBtn
                visible: false
            }

            PropertyChanges {
                target: askBtPermissionsBtn
                visible: false
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }
        },
        State {
            name: "noDevicesFound"
            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: true
            }

            PropertyChanges {
                target: progressMsg
                visible: false
            }

            PropertyChanges {
                target: connectDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: outcomeMsg
                visible: true
                color: "#be7502"
                message: qsTr("Couldn't discover any matching device.")
            }

            PropertyChanges {
                target: pairDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: activateBtBtn
                visible: false
            }

            PropertyChanges {
                target: askBtPermissionsBtn
                visible: false
            }

            PropertyChanges {
                target: discoverAvailableDevicesBtn
                visible: true
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }
        },
        State {
            name: "cantActivateBt"
            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: true
            }

            PropertyChanges {
                target: progressMsg
                visible: false
            }

            PropertyChanges {
                target: connectDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: outcomeMsg
                visible: true
                color: "#be7502"
                message: qsTr("Couldn't activate Bluetooth.")
            }

            PropertyChanges {
                target: pairDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: discoverAvailableDevicesBtn
                visible: false
            }

            PropertyChanges {
                target: askBtPermissionsBtn
                visible: false
            }

            PropertyChanges {
                target: activateBtBtn
                visible: true
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }
        },
        State {
            name: "btPermissionsDenied"
            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: true
            }

            PropertyChanges {
                target: progressMsg
                visible: false
            }

            PropertyChanges {
                target: connectDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: outcomeMsg
                color: "#be7502"
                message: qsTr("Couldn't acquire the necessary permissions.")
            }

            PropertyChanges {
                target: pairDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: discoverAvailableDevicesBtn
                visible: false
            }

            PropertyChanges {
                target: activateBtBtn
                visible: false
            }

            PropertyChanges {
                target: askBtPermissionsBtn
                visible: true
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }
        },
        State {
            name: "oneDeviceFound"
            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: outcomeMsgArea
                visible: true
            }

            PropertyChanges {
                target: progressMsg
                visible: false
            }

            PropertyChanges {
                target: connectDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: discoverAvailableDevicesBtn
                visible: false
            }

            PropertyChanges {
                target: outcomeMsg
                visible: true
                message: qsTr("Found device ") + currentDeviceName
            }

            PropertyChanges {
                target: pairDeviceBtn
                visible: false
            }

            PropertyChanges {
                target: activateBtBtn
                visible: false
            }

            PropertyChanges {
                target: askBtPermissionsBtn
                visible: false
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: stopDeviceDiscoveryBtn
                visible: false
            }
        },
        State {
            name: "definingConnectionPolicy"
            PropertyChanges {
                target: outcomeMsgArea
                visible: false
            }

            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: newDevicesLabel
                message: qsTr("Select the device you want to connect:")
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: true
            }

            PropertyChanges {
                target: connectKnownDeviceBtn
                text: qsTr("Known device")
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: false
            }
        },
        State {
            name: "selectingKnownDeviceToConnect"
            PropertyChanges {
                target: outcomeMsgArea
                visible: false
            }

            PropertyChanges {
                target: progressMsgArea
                visible: false
            }

            PropertyChanges {
                target: newDevicesListFrame
                visible: false
            }

            PropertyChanges {
                target: connectionPolicyFrame
                visible: false
            }

            PropertyChanges {
                target: knownDevicesListFrame
                visible: true
            }
        }
    ]
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
