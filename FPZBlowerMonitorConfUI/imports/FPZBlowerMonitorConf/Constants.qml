pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property int width: 1024
    readonly property int height: 600
    readonly property int contentHeight: 480

    readonly property color colorWarning: "#E0B743"
    readonly property color colorError: "#E65265"
    readonly property color colorSuccess: "#58D544"

    readonly property FontLoader fontMainMediumSrc: FontLoader {
        source: "../../fonts/Quicksand-Medium.ttf"
    }
    readonly property FontLoader fontMainBoldSrc: FontLoader {
        source: "../../fonts/Quicksand-Bold.ttf"
    }

    readonly property FontLoader fontMonoRegularSrc: FontLoader {
        source: "../../fonts/CourierPrime-Regular.ttf"
    }
    readonly property FontLoader fontMonoBoldSrc: FontLoader {
        source: "../../fonts/CourierPrime-Bold.ttf"
    }

    //property alias fontDirectory: directoryFontLoader.fontDirectory
    //property alias relativeFontDirectory: directoryFontLoader.relativeFontDirectory

    /* Edit this comment to add your custom font */
    readonly property font fontMain: Qt.font({
                                                 family: fontMainBoldSrc.name,
                                                 pixelSize: 18,
                                                 weight: Font.Bold
                                             })
    readonly property font fontMainM: Qt.font({
                                                  family: fontMainBoldSrc.name,
                                                  pixelSize: 24,
                                                  weight: Font.Bold
                                              })
    readonly property font fontMainS: Qt.font({
                                                  family: fontMainBoldSrc.name,
                                                  pixelSize: 14,
                                                  weight: Font.Bold
                                              })

    readonly property font fontMainL: Qt.font({
                                                  family: fontMainMediumSrc.name,
                                                  pixelSize: 30,
                                                  weight: Font.Medium
                                              })

    readonly property font fontMonoL: Qt.font({
                                                  family: fontMonoBoldSrc.name,
                                                  pixelSize: 36,
                                                  weight: Font.Bold
                                              })
    readonly property color backgroundColor: "#ffffff"

    /*property DirectoryFontLoader directoryFontLoader: DirectoryFontLoader {
        id: directoryFontLoader
    }*/
}
