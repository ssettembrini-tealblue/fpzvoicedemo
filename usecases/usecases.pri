!android {
    QT += serialbus
    qtConfig(modbus-serialport): QT += serialport

    HEADERS += \
		$$PWD/client_actions.h \
        $$PWD/inveor_store.h \
        $$PWD/../libs/inveor-lib/src/inveor_inverter.h \
        $$PWD/../libs/inveor-lib/src/writeregistermodel.h

    SOURCES += \
        $$PWD/inveor_store.cpp \
        $$PWD/../libs/inveor-lib/src/inveor_inverter.cpp \
        $$PWD/../libs/inveor-lib/src/writeregistermodel.cpp
}

include ($${PWD}/../libs/easy-bluetooth/easy-bluetooth.pri)
android {

HEADERS += \
    $$PWD/client_actions.h \
    $$PWD/connect_device_via_bt.h \
    $$PWD/bt_store.h \
    $$PWD/device_store.h \
    $$PWD/read_current_params.h \
    $$PWD/voice_store.h \


SOURCES += \
    $$PWD/connect_device_via_bt.cpp \
    $$PWD/bt_store.cpp \
    $$PWD/device_store.cpp \
    $$PWD/read_current_params.cpp \
    $$PWD/voice_store.cpp \

}

RESOURCES += \
    $$PWD/usecases.qrc
