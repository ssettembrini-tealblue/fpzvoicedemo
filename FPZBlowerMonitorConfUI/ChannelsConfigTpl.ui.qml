import QtQuick 2.15
import FPZBlowerMonitorConf 1.0
import "businesscontrols"
import "businesscontrols/themes"

Item {
    width: Constants.width
    height: Constants.height

    TemplateBg {
        id: templateBg
        anchors.fill: parent

        Row {
            spacing: 16
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            ChConfigForm {
                id: ch1ConfigForm
                chNum: "Channel 1"
            }
            ChConfigForm {
                id: ch2ConfigForm
                chNum: "Channel 2"
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
