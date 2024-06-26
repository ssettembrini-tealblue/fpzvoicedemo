#include "inveor_store.h"
#include <QStandardItemModel>
#include<QDebug>
#include<QTimer>
#include<QGuiApplication>
InveorStore::InveorStore(ClientActions* clientActions,QObject *parent)
    :QObject(parent)
{
    m_timer= new QTimer(this);
    m_clientActions= clientActions;
    m_inveor_inverter.setupConnection(120,"/dev/ttyAMA3",
                                      BaudRate::Baud38400,
                                      Parity::NoParity,
                                      DataBits::Data8,
                                      StopBits::OneStop);


    //Handling Digital Output 1 with 13 -> Digital Input 1
    //m_inveor_inverter.writeDO1function(13);
    m_inveor_inverter.writePidNominalValueMin(0);
    m_inveor_inverter.writePidNominalValueMax(3);



    connect(m_clientActions, &ClientActions::connectDevice,this,[this](){
        m_inveor_inverter.connect();
        emit connStatusChanged();
        emit commStatusChanged();
        if (m_inveor_inverter.isConnected()){m_timer->start(1000);}
    });
    connect(m_clientActions, &ClientActions::disconnectDevice,this,[this](){
        m_timer->stop();
        m_inveor_inverter.disconnect();
        emit connStatusChanged();
        emit commStatusChanged();

    });
    connect(m_timer, &QTimer::timeout,this,[this](){
        readValues();
    });
    connect(m_clientActions, &ClientActions::quitApp,this,[](){
        QCoreApplication::quit();
    });
    connect(m_clientActions, &ClientActions::writeNominalFrequency,this,[this](uint value){
        //if(value<40){

        setNominalFrequency(value);
        m_inveor_inverter.writeNominalFrequency(value);
        //}
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::stopBlower,this,[this](){
        setNominalFrequency(0);
        m_inveor_inverter.writeNominalFrequency(0);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::startBlower,this,[this](){
        if(actualFrequency()==0){//if(nomFrequency()==0){
            setNominalFrequency(10);
            //setNominalFrequency(10);//minFrequency());
            m_inveor_inverter.writeNominalFrequency(10);//minFrequency());
            emit commStatusChanged();
        }
    });
    connect(m_clientActions, &ClientActions::increaseNominalFrequency,this,[this](uint step){
        uint increasedvalue= nomFreq() + step ;
        qDebug() << "nomFreq() = " << nomFreq();
        qDebug() << "nominal increased value = " << increasedvalue;
        setNominalFrequency(increasedvalue);
        m_inveor_inverter.writeNominalFrequency(increasedvalue);

        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::decreaseNominalFrequency,this,[this](uint step){

        if(nomFreq()>=step){

            uint decreasedvalue = nomFreq() - step;
            setNominalFrequency(decreasedvalue);
            m_inveor_inverter.writeNominalFrequency(decreasedvalue);
            emit commStatusChanged();
        }
        else{
            setNominalFrequency(0);
            m_inveor_inverter.writeNominalFrequency(0);
            emit commStatusChanged();
        }

    });
    connect(m_clientActions, &ClientActions::startDO2,this,[this](){
        setDigOut2(1);
        m_inveor_inverter.writeDO2(1);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::stopDO2,this,[this](){
        setDigOut2(0);
        m_inveor_inverter.writeDO2(0);
        emit commStatusChanged();
    });

    connect(m_clientActions, &ClientActions::writeMaxFrequency,this,[this](uint value){
        m_inveor_inverter.writeMaxFrequency(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeMinFrequency,this,[this](uint value){
        m_inveor_inverter.writeMinFrequency(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeOperatingMode,this,[this](uint value){
        m_inveor_inverter.writeOperatingMode(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeSettingSetpoint,this,[this](uint value){
        m_inveor_inverter.writeSetpointSet(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeApprovalSetpoint,this,[this](uint value){
        m_inveor_inverter.writeSetpointApproved(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writePidActualValue,this,[this](uint value){
        m_inveor_inverter.writePidActualValue(value);
        emit pidActualValueChanged(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writePidNominalValue,this,[this](uint value){
        m_inveor_inverter.writePidNominalValue(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeAnalogicInput1Type,this,[this](uint value){
        m_inveor_inverter.writeAnalogicInput1Type(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeAnalogicInput1Min,this,[this](uint value){
        m_inveor_inverter.writeAnalogicInput1Min(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeAnalogicInput1Max,this,[this](uint value){
        m_inveor_inverter.writeAnalogicInput1Max(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeDO1function,this,[this](uint value){
        m_inveor_inverter.writeDO1function(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeDO1on,this,[this](uint value){
        m_inveor_inverter.writeDO1on(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeDO1off,this,[this](uint value){
        m_inveor_inverter.writeDO1off(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeDO2function,this,[this](uint value){
        m_inveor_inverter.writeDO2function(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeDO2on,this,[this](uint value){
        m_inveor_inverter.writeDO2on(value);
        emit commStatusChanged();
    });
    connect(m_clientActions, &ClientActions::writeDO2off,this,[this](uint value){
        m_inveor_inverter.writeDO2off(value);
        emit commStatusChanged();
    });
}

QString InveorStore::connStatus()
{
    return m_inveor_inverter.status();
}

QString InveorStore::msgStatus()
{
    return m_inveor_inverter.statusMsg();
}

uint InveorStore::minFrequency()
{
    return m_inveor_inverter.minFrequency();
}

uint InveorStore::maxFrequency()
{
    return m_inveor_inverter.maxFrequency();
}

uint InveorStore::operatingMode()
{
    return m_inveor_inverter.operatingMode();
}

uint InveorStore::settingSetpoint()
{
    return m_inveor_inverter.setpointSet();
}

uint InveorStore::approvalSetpoint()
{
    return m_inveor_inverter.setpointApproved();
}

uint InveorStore::pidActualValue()
{
    return m_inveor_inverter.pidActualValue();
}

uint InveorStore::statusWord()
{
    return m_inveor_inverter.statusWord();
}

uint InveorStore::actualFrequency()
{
    return m_inveor_inverter.actualFrequency();
}

uint InveorStore::motorVoltage()
{
    return m_inveor_inverter.motorVoltage();
}

uint InveorStore::motorCurrent()
{
    return m_inveor_inverter.motorCurrent();
}

uint InveorStore::gridVoltage()
{
    return m_inveor_inverter.gridVoltage();
}

uint InveorStore::innerTemp()
{
    return m_inveor_inverter.innerTemperature();
}

uint InveorStore::pidNominalValue()
{
    return m_inveor_inverter.pidNominalValue();
}

uint InveorStore::analogicInput1Type()
{
    return m_inveor_inverter.analogicInput1Type();
}

uint InveorStore::analogicInput1Min()
{
    return m_inveor_inverter.analogicInput1Min();
}

uint InveorStore::analogicInput1Max()
{
    return m_inveor_inverter.analogicInput1Max();
}

uint InveorStore::nomFreq() const
{
    return m_nomFreq;
}

void InveorStore::setNominalFrequency(uint nominalFrequency)
{
    if (m_nomFreq == nominalFrequency)
        return;

    m_nomFreq = nominalFrequency;
    emit nominalFrequencyChanged(m_nomFreq);
}

void InveorStore::setDigOut2(bool digout2)
{
    if (m_digOut2 == digout2)
        return;

    m_digOut2 = digout2;
    //emit digOut2Changed(m_digOut2);
}

void InveorStore::readValues(){
    QTimer::singleShot(1, this, [this](){
        if(m_inveor_inverter.readActualFrequency()){
            emit actualFrequencyChanged();
        }
    });
    QTimer::singleShot(2, this, [this](){
        if(m_inveor_inverter.readMotorVoltage()){
            emit motorVoltageChanged();
        }
    });
    QTimer::singleShot(3, this, [this](){
        if(m_inveor_inverter.readMotorCurrent()){
            emit motorCurrentChanged();
        }
    });
    if(m_inveor_inverter.readMinFrequency()){
        emit minFrequencyChanged(m_inveor_inverter.minFrequency());
    }
    if(m_inveor_inverter.readMaxFrequency()){
        emit maxFrequencyChanged(m_inveor_inverter.maxFrequency());
    }
    if(m_inveor_inverter.readOperatingMode()){
        emit operatingModeChanged(m_inveor_inverter.operatingMode());
    }
    if(m_inveor_inverter.readSetpointSet()){
        emit settingSetpointChanged(m_inveor_inverter.setpointSet());
    }
    if(m_inveor_inverter.readSetpointApproved()){
        emit approvalSetpointChanged(m_inveor_inverter.setpointApproved());
    }
    if(m_inveor_inverter.readPidActualValue()){
        emit pidActualValueChanged(m_inveor_inverter.pidActualValue());
    }
    if(m_inveor_inverter.readStatusWord()){
        emit statusWordChanged();
    }
    if(m_inveor_inverter.readGridVoltage()){
        emit gridVoltageChanged();
    }
    if(m_inveor_inverter.readInnerTemperature()){
        emit innerTempChanged();
    }
    if(m_inveor_inverter.readPidNominalValue()){
        emit pidNominalValueChanged(m_inveor_inverter.pidNominalValue());
    }
    if(m_inveor_inverter.readAnalogicInput1Type()){
        emit analogicInput1TypeChanged(m_inveor_inverter.analogicInput1Type());
    }
    if(m_inveor_inverter.readAnalogicInput1Min()){
        emit analogicInput1MinChanged(m_inveor_inverter.analogicInput1Min());
    }
    if(m_inveor_inverter.readAnalogicInput1Max()){
        emit analogicInput1MaxChanged(m_inveor_inverter.analogicInput1Max());
    }
    emit connStatusChanged();
    emit commStatusChanged();
}

bool InveorStore::digOut2() const
{
    return m_digOut2;
}
