import QtQuick 2.15
import QtGraphicalEffects 1.0

Item {
    id: root
    implicitHeight: childrenRect.height
    implicitWidth: childrenRect.width
    DropShadow {
        source: rectangle
        anchors.fill: rectangle
        radius: 8
        verticalOffset: 8
        color: "#40000000"
    }
    Rectangle {
        id: rectangle
        color: ThemeConstants.colorReaderBg
        anchors.fill: parent
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:180;width:160}
}
##^##*/

