#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardItemModel>
#include <QScxmlStateMachine>
#include <QTimer>
#include <QQmlFileSelector>
#include <QFileSelector>
#include <QSharedPointer>
#include <cstdlib>
//#include <btmanager.h>
//#include <btscanner.h>
//#include <btreceiver.h>
//#include "bluetooth_services_model.h"
#include "../usecases/inveor_store.h"
#include "../usecases/mock_inveor_store.h"
#include "../usecases/voice_store.h"
#include "../usecases/client_actions.h"
//#include "devices_store.h"
//#include "bt_store.h"
#ifdef Q_OS_IOS
#include <QQmlExtensionPlugin>
#include <QtPlugin>
#endif

//Q_DECLARE_METATYPE(WriteRegisterModel*);

int main(int argc, char *argv[])
{
    bool uiDemoMode{0};
    bool logicDemoMode{0};
    bool dataDemoMode{0};
    bool useTheme2{0};

    QCoreApplication::setOrganizationName("FPZ");
    QCoreApplication::setApplicationName("Blower Monitor");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
#ifdef Q_OS_IOS
    Q_IMPORT_PLUGIN(QtQuickControls1Plugin);
    qobject_cast<QQmlExtensionPlugin*>(qt_static_plugin_QtQuickControls1Plugin().instance())->registerTypes("QtQuick.Controls");
#endif
    auto clientActions = new ClientActions(&app);

    //changed INVEOR TO MOCK INVEOR (SPS 2023)
    auto inveorStore = new InveorStore(clientActions,&app);
    auto voiceStore = new VoiceStore(clientActions,&app);
    //clientActions->connectDevice();
    QQmlApplicationEngine engine;

    if (useTheme2) {
        QQmlFileSelector* selector = new QQmlFileSelector(&engine);
        selector->setExtraSelectors(QStringList() << "theme2");
    }
    engine.rootContext()->setContextProperty("clientActions", clientActions);
    
    qRegisterMetaType<WriteRegisterModel*>();
    
    engine.rootContext()->setContextProperty("inveorStore", inveorStore);
    engine.rootContext()->setContextProperty("voiceStore", voiceStore);
    
    QUrl url;
    if (uiDemoMode) {
        url = QStringLiteral("qrc:/FPZBlowerMonitorConfUI/FPZBlowerMonitorConfClientMock.qml");
        //url = QStringLiteral("qrc:/views/ReadInveorParamsView.qml");
    }
    else {
        //engine.rootContext()->setContextProperty("connectDeviceViaBtSM", connectDeviceViaBtSM);
        url = QStringLiteral("qrc:/main.qml");
        //connectDeviceViaBtSM->start();
    }
    engine.load(url);

    return app.exec();
}
