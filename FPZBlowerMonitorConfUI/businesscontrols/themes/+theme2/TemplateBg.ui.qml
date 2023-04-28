import QtQuick 2.15

Rectangle {
    id: root
    color: "#3C4346"
    Image {
        anchors.fill: parent
        source: "+theme2/images/templatebg-pattern.png" // adding +theme2 is a workaround. -> check if it is a bug, or needs a different setup
        fillMode: Image.Tile
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

