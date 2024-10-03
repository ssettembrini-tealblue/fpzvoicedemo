QT += core gui qml quick quickcontrols2 svg scxml

CONFIG += c++11

include(./usecases/usecases.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include ($${PWD}/../easy-bluetooth/easy-bluetooth.pri)

SOURCES += app/main.cpp
RESOURCES += ./FPZBlowerMonitorConfUI/ui.qrc     app/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
macx: QML_IMPORT_PATH += "FPZBlowerMonitorConfUI/imports"
win32: QML_IMPORT_PATH += "FPZBlowerMonitorConfUI/imports"
# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android {
    ANDROID_ABIS = armeabi-v7a arm64-v8a

    DISTFILES += android/AndroidManifest.xml

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

ios { # fixes for Quick Controls Extras 1.4
    QT += gui-private
    LIBS += $$(QTDIR)/qml/QtQuick/Controls/libqtquickcontrolsplugin.a
    RESOURCES += FPZBlowerMonitorConfUI/controls1.qrc }
