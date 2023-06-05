#include "mock_inveor_store.h"
#include <QStandardItemModel>
#include<QDebug>
#include<QTimer>
#include<QGuiApplication>
MockInveorStore::MockInveorStore(ClientActions* clientActions,QObject *parent)
    :QObject(parent)
{
    m_timer= new QTimer(this);
    m_clientActions= clientActions;
    setMotorVoltage(500);
    setMotorCurrent(30);
    setNominalFrequency(20);

    connect(m_clientActions, &ClientActions::connectDevice,this,[this](){
        emit connStatusChanged();
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::disconnectDevice,this,[this](){
        m_timer->stop();
        emit connStatusChanged();
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::quitApp,this,[](){
        QCoreApplication::quit();
    });
    connect(m_clientActions, &ClientActions::writeNominalFrequency,this,[this](uint value){
        //        qDebug() << "MOCK INVEOR REACHED";
        //        qDebug() << "VOLTAGE" << motorVoltage() <<" - CURRENT "<< motorCurrent();
        setNominalFrequency(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::stopBlower,this,[this](){
        setNominalFrequency(0);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::startBlower,this,[this](){
        if(nomFreq()==0){
            setNominalFrequency(10);
        }
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::increaseNominalFrequency,this,[this](uint step){
        uint sum= step + nomFreq();
        if(sum>m_maxFrequency){
            setNominalFrequency(m_maxFrequency);
        }
        else{
            setNominalFrequency(sum);
        }
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::decreaseNominalFrequency,this,[this](uint step){
        if(nomFreq()>step){
            setNominalFrequency(nomFreq() - step);
        }
        else{
            setNominalFrequency(0);
        }
        emit commStatusChanged();
    });

}

uint MockInveorStore::motorVoltage()
{
    return m_motorVoltage;
}

uint MockInveorStore::motorCurrent()
{
    return m_motorCurrent;
}

uint MockInveorStore::nomFreq() //const
{
    return m_nomFreq;
}

void MockInveorStore::setMotorVoltage(uint newMotorVoltage)
{
    if (m_motorVoltage == newMotorVoltage)
        return;
    m_motorVoltage = newMotorVoltage;
    emit motorVoltageChanged(newMotorVoltage);
}


void MockInveorStore::setMotorCurrent(uint newMotorCurrent)
{
    if (m_motorCurrent == newMotorCurrent)
        return;
    m_motorCurrent = newMotorCurrent;
    emit motorCurrentChanged(newMotorCurrent);
}

void MockInveorStore::setNominalFrequency(uint nominalFrequency)
{
    if (m_nomFreq == nominalFrequency)
        return;

    m_nomFreq = nominalFrequency;
    emit nominalFrequencyChanged(m_nomFreq);
}
