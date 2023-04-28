#include "read_current_params.h"

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QByteArray>
#include<QString>
#include <QGuiApplication>

ReadCurrentParams::ReadCurrentParams(ClientActions* clientActions, BtStore* btStore, DeviceStore* deviceStore, QObject *parent) : QObject(parent)
{
    m_clientActions = clientActions;
    m_deviceStore = deviceStore;
    m_btStore = btStore;
    QObject::connect(m_btStore->receiver(), &BtReceiver::packet, this, [this] (const QByteArray data) {
        QJsonDocument docJson = QJsonDocument::fromJson(data);
        QJsonObject jsonObject = docJson.object();
        QJsonArray jsonArray = jsonObject["channels"].toArray();

        m_deviceStore->setChannel1Value(jsonArray.at(0).toObject()["value"].toDouble());
        m_deviceStore->setChannel2Value(jsonArray.at(1).toObject()["value"].toDouble());
        m_deviceStore->setChannel3Value(jsonArray.at(2).toObject()["value"].toDouble());
        m_deviceStore->setChannel4Value(jsonArray.at(3).toObject()["value"].toDouble());
        m_deviceStore->setChannel5Value(jsonArray.at(4).toObject()["value"].toDouble());
        m_deviceStore->setChannel6Value(jsonArray.at(5).toObject()["value"].toDouble());
    });
    connect(m_clientActions, &ClientActions::quitApp,this,[this](){
        m_btStore->receiver()->stopReceiver();
        QTimer::singleShot(300, this, [](){
            QCoreApplication::quit();

        });
    });
   // connect(m_btStore->receiver(),&BtReceiver::)
}
