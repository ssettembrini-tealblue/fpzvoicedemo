import QtQuick 2.15
import QtQuick.Window 2.15
import FPZBlowerMonitorConf 1.0

Window {
    visible: true
    width: Constants.width
    height: Constants.height
    property bool sensorDataIsComing: false
    property string testDeviceName

    ReadCurrentParamsCustomTpl {
        id: readCurrentParamsView
        state: "init"
        anchors.fill: parent
        visible: true
        connectedDeviceName.text: testDeviceName
        Timer {
            id: sensorDataRequestingTimer
            interval: 2000
            onTriggered: {
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
                parent.customPressureReader.value = Math.ceil(900 * Math.random())
                parent.customPressureReaderFlatBg.value = Math.ceil(900 * Math.random())
                parent.customTripleReader.leftGauge.value = Math.ceil(100 * Math.random())
                parent.customTripleReader.rightGauge.value = Math.ceil(1000 * Math.random())
                parent.customTripleReader.bottomGauge.value = Math.ceil(100 * Math.random())
                sensorDataOldTimer.start()
            }
        }
        Timer {
            id: sensorDataOldTimer
            interval: 5000
            onTriggered: {
                sensorDataSyncingTimer.stop()
                parent.state ="sensorDataOld"
                sensorDataLastUpdateTimer.start()
                sensorDataTooOldTimer.start()
            }
        }
        Timer {
            id: sensorDataLastUpdateTimer
            interval: 1000
            repeat: true
            onTriggered: parent.sensorDataLastUpdateTime.text = parseInt(parent.sensorDataLastUpdateTime.text)+1
        }
        Timer {
            id: sensorDataTooOldTimer
            interval: 5000
            onTriggered: {
                parent.state = "sensorDataTooOld"
            }
        }
    }
    Component.onCompleted: sensorDataRequestingTimer.start()
}
