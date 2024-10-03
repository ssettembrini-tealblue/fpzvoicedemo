import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtScxml 5.15
import FPZBlowerMonitorConf 1.0
import "FPZBlowerMonitorConfUI"
import "./views"
import "FPZBlowerMonitorConfUI/businesscontrols"

Window {
    id: root
    visible: true
    width: minimumWidth
    height: minimumHeight
    minimumWidth: Constants.width
    minimumHeight: Constants.height
  //  Component.onCompleted: findDeviceNameDialog.open()
//    Dialog {
//        id: findDeviceNameDialog
//        modal: true
//        closePolicy: Popup.NoAutoClose
//        anchors.centerIn: parent
//        width: 400
//        Column {
//            width: parent.width
//            Label {
//                text: "Name of device to search for:"
//            }
//            TextField {
//                id: findDeviceNameField
//                text: "FPZ 1"
//                width: parent.width
//            }
//            Label {
//                text: "Uuid of service:"
//            }
//            TextField {
//                id: findServiceUuidField
//                text: "e8e10f95-1a70-4b27-9ccf-02010264e9c3"
//                width: parent.width
//            }
//        }
//        standardButtons: Dialog.Ok
//        onAccepted: clientActions.startBtDeviceDiscovery(findDeviceNameField.text, findServiceUuidField.text)
//    }
//    EventConnection {
//        stateMachine: connectDeviceViaBtSM
//        events: "outcome.device_connected"
//        onOccurred: {
//            connectDeviceViaBtView.visible = false
//            readCurrentParamsView.visible = true
//        }
//    }
//    EventConnection {
//        stateMachine: connectDeviceViaBtSM
//        events: "outcome.device_disconnected"
//        onOccurred: {
//            connectDeviceViaBtView.visible = true
//            readCurrentParamsView.visible = false
//        }
//    }
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
//        StatusBar {
//            id: statusBar
//            Layout.fillWidth: true
//            connectedDeviceName.text: deviceStore.currentDeviceName
//            sensorDataUpdateStatus.secs.text: deviceStore.sensorDataLastUpdated
//            sensorDataUpdateStatus.state: switch(deviceStore.sensorDataStatus) {
//                                          case SensorDataStatus.REGULAR: "running"; break;
//                                          case SensorDataStatus.OLD: "alert"; break;
//                                          case SensorDataStatus.TOO_OLD: "error"; break;
//                                          default: ""
//                                          }

//            btConnectionIndicator.state: switch(btStore.receiverStatus) {
//                                         case BtReceiverStatus.DISCONNECTED: ""; break;
//                                         case BtReceiverStatus.SEARCHING: "searching"; break;
//                                         case BtReceiverStatus.CONNECTED: "connected"; break;
//                                         case BtReceiverStatus.DISABLED: "disabled"; break;
//                                         default: "disabled"
//                                         }
        //}
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
//            V.ConnectDeviceViaBtView {
//                id: connectDeviceViaBtView
//                anchors.fill: parent
//                Text { // DEBUG
//                    visible: false
//                    anchors.bottom: parent.bottom
//                    text: parent.state
//                }
//                Connections {
//                    target: connectDeviceViaBtSM
//                    onReachedStableState: connectDeviceViaBtView.state = connectDeviceViaBtSM.activeStateNames()[0]
//                }
//                state: "init"
//            }

             ReadInveorParamsVoiceView{
                id: readCurrentParamsView
                //anchors.fill: parent
                visible: true
                //actionArea.visible: false
//                state: switch(deviceStore.sensorDataStatus) {
//                       case SensorDataStatus.REGULAR: "init"; break;
//                       case SensorDataStatus.OLD: "sensorDataOld"; break;
//                       case SensorDataStatus.TOO_OLD: "sensorDataTooOld"; break;
//                       default: "init"
//                       }
//                Text { // DEBUG
//                    visible: false
//                    anchors.bottom: parent.bottom
//                    text: parent.state
//                    color: "white"
//                }
            }
        }
    }
}
