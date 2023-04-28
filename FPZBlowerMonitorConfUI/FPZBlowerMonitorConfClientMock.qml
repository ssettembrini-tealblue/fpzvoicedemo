import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import FPZBlowerMonitorConf 1.0
import "businesscontrols"

Window {
    id: root
    visible: true
    width: Constants.width
    height: Constants.height
    property bool sensorDataIsComing: false
    property string testDeviceName
    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        StatusBar {
            id: statusBar
            Layout.fillWidth: true
            connectedDeviceName.text: root.testDeviceName
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            ReadCurrentParamsTpl {
                id: readCurrentParamsView
                state: "init"
                anchors.fill: parent
                visible: false//true
                ambientTempReader.onMinErrorValueChanged: console.log(ambientTempReader.minErrorValue)
                ambientTempReader.onStateChanged: console.log(ambientTempReader.state)
                Timer {
                    id: sensorDataRequestingTimer
                    interval: 2000
                    onTriggered: {
                        connectDeviceViaBtView.visible = false
                        parent.visible = true
                        parent.state = "sensorDataRequesting"
                        sensorDataSyncingTimer.start()
                    }
                }
                Timer {
                    id: sensorDataSyncingTimer
                    interval: 1000
                    repeat: true
                    onTriggered: {
                        parent.state = "sensorDataSyncing"
                        statusBar.sensorDataUpdateStatus.state = "running"
                        parent.ambientTempReader.value = (70 * Math.random()).toFixed(2)
                        parent.ambientPressureReader.value = Math.ceil(900 * Math.random())
                        parent.outletTempReader.value = (70 * Math.random()).toFixed(2)
                        parent.outletPressureReader.value = Math.ceil(900 * Math.random())
                        parent.inletTempReader.value = (70 * Math.random()).toFixed(2)
                        parent.inletPressureReader.value = Math.ceil(900 * Math.random())
                        sensorDataOldTimer.start()
                    }
                }
                Timer {
                    id: sensorDataOldTimer
                    interval: 5000
                    onTriggered: {
                        sensorDataSyncingTimer.stop()
                        parent.state ="sensorDataOld"
                        statusBar.sensorDataUpdateStatus.state = "alert"
                        sensorDataLastUpdateTimer.start()
                        sensorDataTooOldTimer.start()
                    }
                }
                Timer {
                    id: sensorDataLastUpdateTimer
                    interval: 1000
                    repeat: true
                    onTriggered: {
                        statusBar.sensorDataUpdateStatus.secs.text = parseInt(statusBar.sensorDataUpdateStatus.secs.text)+1
                    }
                }
                Timer {
                    id: sensorDataTooOldTimer
                    interval: 5000
                    onTriggered: {
                        parent.state = "sensorDataTooOld"
                        statusBar.sensorDataUpdateStatus.state = "error"
                    }
                }
            }

            ConnectDeviceViaBtTpl {
                id: connectDeviceViaBtView
                anchors.fill: parent
                visible: true
                Timer {
                    id: discoveringDevicesTimer
                    interval: 1000
                    onTriggered: {
                        parent.state = "discoveringAvailableDevices"
                        oneDeviceFoundTimer.start()
                    }
                }

                Timer {
                    id: oneDeviceFoundTimer
                    interval: 1000
                    onTriggered: {
                        testDeviceName = "FPZ Blower SN/CX839405850"
                        parent.state = "oneDeviceFound"
                        checkingDeviceIsAlreadyPairedTimer.start()
                        parent.outcomeMsg.text = parent.outcomeMsg.text.arg(testDeviceName)
                    }
                }
                Timer {
                    id: checkingDeviceIsAlreadyPairedTimer
                    interval: 500
                    onTriggered: {
                        parent.state = "checkingDeviceIsAlreadyPaired"
                        connectingDeviceTimer.start()
                        parent.progressMsg.text = parent.progressMsg.text.arg(testDeviceName)
                    }
                }
                //        Timer {
                //            id: pairingDeviceTimer
                //            interval: 2000
                //            onTriggered: {
                //                parent.state = "pairingDevice"
                //                connectingDeviceTimer.start()
                //                parent.progressMsg.text = parent.progressMsg.text.arg(testDeviceName)
                //            }
                //        }
                Timer {
                    id: connectingDeviceTimer
                    interval: 1000
                    onTriggered: {
                        parent.state = "connectingDevice"
                        deviceConnectedTimer.start()
                        parent.progressMsg.text = parent.progressMsg.text.arg(testDeviceName)
                    }
                }
                Timer {
                    id: deviceConnectedTimer
                    interval: 1000
                    onTriggered: {
                        parent.state = "deviceConnected"
                        sensorDataRequestingTimer.start()
                        parent.outcomeMsg.text = parent.outcomeMsg.text.arg(testDeviceName)
                    }
                }
                Component.onCompleted: {
                    //sensorDataRequestingTimer.start()

                    state = "activatingBt"
                    discoveringDevicesTimer.start()
                }
                //onDeviceItemClicked: {state = "wantPairDevice";wantPairDeviceDialog.deviceNumber = index + 1}
                //wantPairDeviceDialog.onAccepted: {state = "deviceWasPaired";currentParametersTimer.start()}

            }

        }
    }
    //Component.onCompleted: sensorDataRequestingTimer.start()
}
