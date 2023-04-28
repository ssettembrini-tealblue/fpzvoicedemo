#include "inveor_inverter.h"
#include <QDebug>
#include <QTimer>
#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QUrl>
#include <QSerialPort>
#include<cmath>

InveorInverter::InveorInverter()
{
    m_writeRegisterModel = new WriteRegisterModel();
    m_modbusDevice = new QModbusRtuSerialMaster();

}

WriteRegisterModel *InveorInverter::writeRegisterModel() const
{
    return m_writeRegisterModel;
}



void InveorInverter::setStatus(const QString& commStatus){
    if(m_modbusDevice->state()==QModbusDevice::ConnectedState)
    {
        m_connStatus=status::Connected;
    }
    else if(m_modbusDevice->state()==QModbusDevice::UnconnectedState)
    {
        m_connStatus=status::Disconnected;
    }
    else if(m_modbusDevice->state()==QModbusDevice::ConnectingState)
    {
        m_connStatus=status::Connecting;
    }
    else
    {
        m_connStatus=status::Unknown;
    }
    if(commStatus!=""){m_commStatus=commStatus;}
}

void InveorInverter::sendError(modbusError error,QModbusReply *reply)
{
    switch(error){
    case modbusError::ProtocolRead:
        setStatus(QObject::tr("Read response error: %1 (Modbus exception: 0x%2)").
                  arg(reply->errorString()).
                  arg(reply->rawResult().exceptionCode(), -1, 16) );
        break;
    case modbusError::Read:
        setStatus(QObject::tr("Read response error: %1 (code: 0x%2)").
                  arg(reply->errorString()).
                  arg(reply->error(), -1, 16));
        break;
    case modbusError::GenericRead:
        setStatus(QObject::tr("Read error: ") + m_modbusDevice->errorString());
        break;
    case modbusError::ProtocolWrite:
        setStatus(QObject::tr("Write response error: %1 (Modbus exception: 0x%2)")
                  .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));

        break;
    case modbusError::Write:
        setStatus(QObject::tr("Write response error: %1 (code: 0x%2)").
                  arg(reply->errorString()).arg(reply->error(), -1, 16));

        break;
    case modbusError::GenericWrite:
        setStatus("Write error: " + m_modbusDevice->errorString());

        break;
    default:
        setStatus();
        break;
    }
}

QString InveorInverter::status(){
    switch(m_connStatus){
    case status::Connected:
        return QString("Connected");
        break;
    case status::Disconnected:
        return QString("Disconnected");
        break;
    case status::ForcedDisconnect:
        return QString("Forced Disconnected");
        break;
    case status::Connecting:
        return QString("Connecting");
        break;
    default:
        return QString("Unknown Status");
        break;
    }
}

QString InveorInverter::statusMsg(){
    return m_commStatus;
}

bool InveorInverter::isConnected(){
    return m_connStatus==status::Connected ? true : false;
}

QModbusDataUnit InveorInverter::readRequest(int registerType,quint16 numberEntries, int address) const{
    auto table =static_cast<QModbusDataUnit::RegisterType>(4);

    switch(registerType) {
    case 1:
        table = static_cast<QModbusDataUnit::RegisterType>(1);
        break;
    case 2:
        table =static_cast<QModbusDataUnit::RegisterType>(2);
        break;
    case 3:
        table =static_cast<QModbusDataUnit::RegisterType>(3);
        break;
    case 4:
        table =static_cast<QModbusDataUnit::RegisterType>(4);
        break;
    default:
        table =static_cast<QModbusDataUnit::RegisterType>(4);
        break;
    }

    return QModbusDataUnit(table, address, numberEntries);
}

QModbusDataUnit InveorInverter::writeRequest(int registerType,quint16 numberEntries, int address) const
{
    auto table =static_cast<QModbusDataUnit::RegisterType>(4);
    switch(registerType) {
    case 1:
        table = static_cast<QModbusDataUnit::RegisterType>(1);
        break;
    case 2:
        table =static_cast<QModbusDataUnit::RegisterType>(2);
        break;
    case 3:
        table =static_cast<QModbusDataUnit::RegisterType>(3);
        break;
    case 4:
        table =static_cast<QModbusDataUnit::RegisterType>(4);
        break;
    default:
        table =static_cast<QModbusDataUnit::RegisterType>(4);
        break;
    }

    return QModbusDataUnit(table, address, numberEntries);
}


void InveorInverter::connect(){
    qDebug() << Q_FUNC_INFO;

    if (!m_modbusDevice->connectDevice()) {
        setStatus("Connect failed: " + m_modbusDevice->errorString());
        return;
    }
    else{
        setStatus("Connecting Serial");
        int state= m_modbusDevice->state();
        if(state==QModbusDevice::ConnectedState){
            setStatus("Connected");
        }
        setStatus();

    }
}

void InveorInverter::disconnect(){
    setStatus("Disconnecting");

    int state= m_modbusDevice->state();
    if(state==QModbusDevice::ConnectedState){
        m_modbusDevice->disconnectDevice();
        setStatus("Disconnected");
    }

    setStatus();

}

void InveorInverter::setupConnection(int id, QString port, BaudRate baud, Parity par, DataBits data, StopBits stop)
{
    setDeviceId(id);
    setSerialPort(port);
    setBaud(baud);
    setDataBits(data);
    setStopBits(stop);
    setParity(par);

        m_modbusDevice->setTimeout(timeout());
        m_modbusDevice->setNumberOfRetries(numberRetries());

}



void InveorInverter::setBaud(BaudRate baud)
{
    m_baudRate=baud;
    m_modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                           int(baud));
    qDebug()<<int(baud);

}

void InveorInverter::setParity(Parity par)
{
    m_parity=par;
    m_modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                           int(par));
    qDebug()<<int(par);
}

void InveorInverter::setDataBits(DataBits data)
{ 

    m_dataBits=data;
    m_modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                           int(data));
    qDebug()<<int(data);

}

void InveorInverter::setStopBits(StopBits stop)
{
    m_stopBits=stop;
    m_modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                           int(stop));
    qDebug()<<int(stop);

}

void InveorInverter::setSerialPort(QString serialPort)
{
    if (m_serialPort == serialPort)
        return;
    m_serialPort=serialPort;
    m_modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                           serialPort);

}

void InveorInverter::setTcpPort(QString tcpPort)
{
    if (m_tcpPort == tcpPort)
        return;
    m_tcpPort=tcpPort;
}

void InveorInverter::setTcpAddress(QString ipAddress)
{
    if (m_tcpIpAddress == ipAddress)
        return;
    m_tcpIpAddress=ipAddress;
}

void InveorInverter::setNumRetries(int numRetries)
{
    if (m_numRetries == numRetries)
        return;
    m_numRetries=numRetries;
    m_modbusDevice->setNumberOfRetries(m_numRetries);

}

void InveorInverter::setTimeout(int timeout)
{
    if (m_timeout == timeout)
        return;
    m_timeout=timeout;
    m_modbusDevice->setTimeout(m_timeout);

}

bool InveorInverter::writeNominalFrequency(uint value)
{
    qDebug() << Q_FUNC_INFO;
    qint16 val=value*10;
    QBitArray ba(16);
    ba.setBit(0, true);
    ba.setBit(1, true);
    ba.setBit(2, true);
    ba.setBit(3, true);
    ba.setBit(4, true);
    ba.setBit(6, true);
    ba.setBit(7,false);
    ba.setBit(10, true);
    quint16 v = 0;
    for (int i=0; i<16; i++){
        v |= ba.at(i)<<i;
    }
    writeRegisterModel()->m_holdingRegisters.insert(0,v);
    writeRegisterModel()->m_holdingRegisters.insert(1,val);

    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Control Word and Frequency");

    QModbusDataUnit writeUnit = writeRequest(4,2,2001);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeControlWord(uint newControlWord)
{
    QBitArray ba(16);
    ba.setBit(0, cw0());
    ba.setBit(1, cw1());
    ba.setBit(2, cw2());
    ba.setBit(3, cw3());
    ba.setBit(4, cw4());
    ba.setBit(5, cw5());
    ba.setBit(6, cw6());
    ba.setBit(7,cw7());
    ba.setBit(8,cw8());
    ba.setBit(9,cw9());
    ba.setBit(10, cw10());
    ba.setBit(11, cw11());
    ba.setBit(12, cw12());
    ba.setBit(13, cw13());
    ba.setBit(14, cw14());
    ba.setBit(15, cw15());
    quint16 v = 0;
    for (int i=0; i<16; i++){
        v |= ba.at(i)<<i;
    }
    writeRegisterModel()->m_holdingRegisters.insert(0,v);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Control Word");

    QModbusDataUnit writeUnit = writeRequest(4,1,2001);
    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }
    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::readStatusWord()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,1999),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply, this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setStatusWord(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);
                    return 0;
                } else {
                    sendError(modbusError::Read,reply);
                    return 0;
                }

                reply->deleteLater();
            });

        }else{
            delete reply;
        }
    } else {
        sendError(modbusError::GenericRead,reply);
        return 0;
    }
}

bool InveorInverter::readActualFrequency()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,2000),deviceId())) {
        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished,[reply, this](){

                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setReadActualFrequency(unit.value(i)/10);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);
        return 0;
    }

}


bool InveorInverter::readMotorVoltage()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");

    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3323),deviceId())) {
        if (!reply->isFinished()){
            QObject::connect(reply, &QModbusReply::finished,[reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {

                        qint16 valueR= (unit.value(i)/pow(2,15))*1500;

                        setMotorVoltage(valueR);
                    }
                    return 1;

                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);
        return 0;
    }

}
bool InveorInverter::readMotorCurrent()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");

    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,1001),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setMotorCurrent(valueR);
                    }
                    return 1;

                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);
        return 0;
    }

}
bool InveorInverter::readTargetFrequency()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");

    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,1004),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*400;
                        setTargetFrequency(valueR);
                    }
                    return 1;

                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }

}

bool InveorInverter::readGridVoltage()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,1005),deviceId())) {
        if (!reply->isFinished()){
            QObject::connect(reply, &QModbusReply::finished,  [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*1000;
                        setGridVoltage(valueR);

                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readInnerTemperature()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,1007),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*200;
                        setInnerTemperature(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput1()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,1015),deviceId())) {
        if (!reply->isFinished()){
            QObject::connect(reply, &QModbusReply::finished,  [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*10;
                        setAnalogicInput1(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput2()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,1016),deviceId())) {
        if (!reply->isFinished()){
            QObject::connect(reply, &QModbusReply::finished,  [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*10;
                        setAnalogicInput2(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }

}

bool InveorInverter::readPidActualValue()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3005),deviceId())) {
        if (!reply->isFinished()){
            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {

                        setPidActualValue(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::writeMaxFrequency(double maxFrequency)
{
    double maxF= (maxFrequency/400)*pow(2,15);
    uint m= static_cast<uint>(maxF);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);

    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Max Frequency");
    QModbusDataUnit writeUnit = writeRequest(4,1,3000);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }
    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeMinFrequency(double minFrequency)
{
    double minF= (minFrequency/400)*pow(2,15);
    uint m= static_cast<uint>(minF);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);

    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Min Frequency");

    QModbusDataUnit writeUnit = writeRequest(4,1,2999);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeOperatingMode(double operatingMode)
{
    writeRegisterModel()->m_holdingRegisters.insert(0,operatingMode);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Operating Mode");

    QModbusDataUnit writeUnit = writeRequest(4,1,3007);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }
    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeSetpointSet(double setpointSet)
{

    writeRegisterModel()->m_holdingRegisters.insert(0,setpointSet);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Setpoint Setting");

    QModbusDataUnit writeUnit = writeRequest(4,1,3004);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeSetpointApproved(double setpointApproved)
{
    writeRegisterModel()->m_holdingRegisters.insert(0,setpointApproved);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Setpoint Approval");

    QModbusDataUnit writeUnit = writeRequest(4,1,3006);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeRotationDirection(double rotationDirection)
{
    double rotD= (rotationDirection/16)*pow(2,15);
    uint m= static_cast<uint>(rotD);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Rotation Direction");

    QModbusDataUnit writeUnit = writeRequest(4,1,3040);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }

}

bool InveorInverter::writePidP(double pidP)
{
    double pP= (pidP/100)*pow(2,15);
    uint m= static_cast<uint>(pP);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID P");

    QModbusDataUnit writeUnit = writeRequest(4,1,3036);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writePidI(double pidI)
{
    double pI= (pidI/100)*pow(2,15);
    uint m= static_cast<uint>(pI);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID I");

    QModbusDataUnit writeUnit = writeRequest(4,1,3037);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writePidD(double pidD)
{
    double pD= (pidD/100)*pow(2,15);
    uint m= static_cast<uint>(pD);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID D");

    QModbusDataUnit writeUnit = writeRequest(4,1,3038);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writePidActualValue(double pidActualValue)
{
    writeRegisterModel()->m_holdingRegisters.insert(0,pidActualValue);

    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID Actual Value");

    QModbusDataUnit writeUnit = writeRequest(4,1,3005);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writePidInversion(double pidInversion)
{
    writeRegisterModel()->m_holdingRegisters.insert(0,pidInversion);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID Inversion");

    QModbusDataUnit writeUnit = writeRequest(4,1,3081);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writePidNominalValue(double pidNominalValue)
{
    double pNV= (pidNominalValue/100)*pow(2,15);
    uint m= static_cast<uint>(pNV);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID Nominal Value");

    QModbusDataUnit writeUnit = writeRequest(4,1,3082);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writePidNominalValueMode(double pidNominalValueMode)
{
    writeRegisterModel()->m_holdingRegisters.insert(0,pidNominalValueMode);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID Nominal Value Mode");

    QModbusDataUnit writeUnit = writeRequest(4,1,3132);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writePidNominalValueMax(double pidNominalValueMax)
{
    double pNVMx= (pidNominalValueMax/100)*pow(2,15);
    uint m= static_cast<uint>(pNVMx);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID Max Nominal Value");

    QModbusDataUnit writeUnit = writeRequest(4,1,3169);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writePidNominalValueMin(double pidNominalValueMin)
{
    double pNVMi= (pidNominalValueMin/100)*pow(2,15);
    uint m= static_cast<uint>(pNVMi);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing PID Min Nominal Value");

    QModbusDataUnit writeUnit = writeRequest(4,1,3168);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput1Type(double analogic1InputType)
{
    writeRegisterModel()->m_holdingRegisters.insert(0,analogic1InputType);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 1 Type");

    QModbusDataUnit writeUnit = writeRequest(4,1,3024);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput1Min(double analogicInput1Min)
{
    double aI1M= (analogicInput1Min/100)*pow(2,15);
    uint m= static_cast<uint>(aI1M);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 1 Min");

    QModbusDataUnit writeUnit = writeRequest(4,1,3025);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput1Max(double analogicInput1Max)
{
    double aI1M= (analogicInput1Max/100)*pow(2,15);
    uint m= static_cast<uint>(aI1M);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 1 Max");

    QModbusDataUnit writeUnit = writeRequest(4,1,3026);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput1Function(double analogicInput1Function)
{

    writeRegisterModel()->m_holdingRegisters.insert(0,analogicInput1Function);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 1 Function");

    QModbusDataUnit writeUnit = writeRequest(4,1,3018);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput1Unit(double analogicInput1Unit)
{
    writeRegisterModel()->m_holdingRegisters.insert(0,analogicInput1Unit);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 1 Unit");

    QModbusDataUnit writeUnit = writeRequest(4,1,3102);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput1UnitMin(double analogicInput1UnitMin)
{
    double aI1UM= (analogicInput1UnitMin/10000)*pow(2,15);
    uint m= static_cast<uint>(aI1UM);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 1 Unit Min");

    QModbusDataUnit writeUnit = writeRequest(4,1,3103);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput1UnitMax(double analogicInput1UnitMax)
{
    double aI1UM= (analogicInput1UnitMax/10000)*pow(2,15);
    uint m= static_cast<uint>(aI1UM);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 1 Unit Max");

    QModbusDataUnit writeUnit = writeRequest(4,1,3104);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput2Type(double analogicInput2Type)
{
    writeRegisterModel()->m_holdingRegisters.insert(0,analogicInput2Type);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 2 Type");

    QModbusDataUnit writeUnit = writeRequest(4,1,3033);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput2Min(double analogicInput2Min)
{
    double aI2M= (analogicInput2Min/100)*pow(2,15);
    uint m= static_cast<uint>(aI2M);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 2 Min");

    QModbusDataUnit writeUnit = writeRequest(4,1,3034);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput2Max(double analogicInput2Max)
{
    double aI2M= (analogicInput2Max/100)*pow(2,15);
    uint m= static_cast<uint>(aI2M);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 2 Max");

    QModbusDataUnit writeUnit = writeRequest(4,1,3035);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput2Function(double analogicInput2Function)
{


    writeRegisterModel()->m_holdingRegisters.insert(0,analogicInput2Function);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 2 Function");

    QModbusDataUnit writeUnit = writeRequest(4,1,3027);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput2Unit(double analogicInput2Unit)
{


    writeRegisterModel()->m_holdingRegisters.insert(0,analogicInput2Unit);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 2 Unit");

    QModbusDataUnit writeUnit = writeRequest(4,1,3105);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput2UnitMin(double analogicInput2UnitMin)
{
    double aI2UM= (analogicInput2UnitMin/10000)*pow(2,15);
    uint m= static_cast<uint>(aI2UM);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 2 Unit Min");

    QModbusDataUnit writeUnit = writeRequest(4,1,3106);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeAnalogicInput2UnitMax(double analogicInput2UnitMax)
{
    double aI2UM= (analogicInput2UnitMax/10000)*pow(2,15);
    uint m= static_cast<uint>(aI2UM);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Analogic Input 2 Unit Max");

    QModbusDataUnit writeUnit = writeRequest(4,1,3107);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeModbusDeviceIndex(double deviceIndex)
{
    double dI= (deviceIndex/247)*pow(2,15);
    uint m= static_cast<uint>(dI);

    writeRegisterModel()->m_holdingRegisters.insert(0,m);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Modbus Device Index");

    QModbusDataUnit writeUnit = writeRequest(4,1,3097);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::writeModbusBaudRate(double baudRate)
{

    writeRegisterModel()->m_holdingRegisters.insert(0,baudRate);
    if (!m_modbusDevice)
        return 0;
    setStatus("Writing Modbus Baud Rate");

    QModbusDataUnit writeUnit = writeRequest(4,1,3109);

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        writeUnit.setValue(i, writeRegisterModel()->m_holdingRegisters[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, deviceId())) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolWrite,reply) ;                     return 0;
                } else if (reply->error() != QModbusDevice::NoError) {
                    sendError(modbusError::Write,reply) ;                     return 0;
                }
                reply->deleteLater();
                return 1;
            });
        } else {
            reply->deleteLater();
        }
    } else {
        sendError(modbusError::GenericWrite,reply) ;       return 0;
    }
}

bool InveorInverter::readPidTargetValue()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,1020),deviceId())) {
        if (!reply->isFinished()){
            QObject::connect(reply, &QModbusReply::finished,  [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setPidTargetValue(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}


bool InveorInverter::readMaxFrequency()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3000),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*400;
                        setMaxFrequency(valueR);

                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readMinFrequency()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,2999),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*400;
                        setMinFrequency(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readOperatingMode()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3007),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setOperatingMode(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readSetpointSet()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3004),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setSetpointSet(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readSetpointApproved()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3006),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setSetpointApproved(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readRotationDirection()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3040),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setRotationDirection(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readPidP()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3036),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setPidP(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readPidI()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3037),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setPidI(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readPidD()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3038),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setPidD(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}
bool InveorInverter::readPidInversion()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3081),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setPidInversion(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readPidNominalValue()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3082),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setPidNominalValue(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readPidNominalValueMode()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3132),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setPidNominalValueMode(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);
        return 0;
    }
}

bool InveorInverter::readPidNominalValueMax()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3169),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setPidNominalValueMax(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readPidNominalValueMin()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3168),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setPidNominalValueMin(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput1Type()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3024),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setAnalogicInput1Type(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput1Min()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3025),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setAnalogicInput1Min(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput1Max()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3026),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setAnalogicInput1Max(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput1Function()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3018),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setAnalogicInput1Function(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput1Unit()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3102),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setAnalogicInput1Unit(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput1UnitMin()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3103),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*10000;
                        setAnalogicInput1UnitMin(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput1UnitMax()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3104),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*10000;
                        setAnalogicInput1UnitMax(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput2Type()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3033),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setAnalogicInput2Type(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput2Min()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3034),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setAnalogicInput2Min(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput2Max()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3035),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*100;
                        setAnalogicInput2Max(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput2Function()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3027),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setAnalogicInput2Function(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput2Unit()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3105),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setAnalogicInput2Unit(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput2UnitMin()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3106),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*10000;
                        setAnalogicInput2UnitMin(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readAnalogicInput2UnitMax()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3107),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*10000;
                        setAnalogicInput2UnitMax(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readModbusDeviceIndex()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3097),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        qint16 valueR= (unit.value(i)/pow(2,15))*247;
                        setModbusDeviceIndex(valueR);
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}

bool InveorInverter::readModbusBaudRate()
{
    if (!m_modbusDevice)
        return 0;
    setStatus("Reading");
    if (auto *reply = m_modbusDevice->sendReadRequest(readRequest(4,1,3109),deviceId())) {

        if (!reply->isFinished()){

            QObject::connect(reply, &QModbusReply::finished, [reply,this](){
                if (!reply)
                    return 0;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                        setModbusBaudRate(unit.value(i));
                    }
                    return 1;
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    sendError(modbusError::ProtocolRead,reply);

                    return 0;
                } else {
                    sendError(modbusError::Read,reply);

                    return 0;
                }

                reply->deleteLater();
            });
        }else{
            delete reply;
        }

    } else {
        sendError(modbusError::GenericRead,reply);                return 0;
    }
}



uint InveorInverter::motorVoltage()
{
    return m_motorVoltage;
}

void InveorInverter::setMotorVoltage(uint newReadVoltage)
{
    if (m_motorVoltage == newReadVoltage)
        return;
    m_motorVoltage=newReadVoltage;
}

uint InveorInverter::motorCurrent()
{
    return m_motorCurrent;
}

void InveorInverter::setMotorCurrent(uint newReadPower)
{
    if (m_motorCurrent == newReadPower)
        return;
    m_motorCurrent=newReadPower;
}

uint InveorInverter::actualFrequency()
{
    return m_actualFrequency;
}
void InveorInverter::setReadActualFrequency(uint newReadFrequency)
{
    if (m_actualFrequency == newReadFrequency)
        return;
    m_actualFrequency=newReadFrequency;
}

uint InveorInverter::statusWord()
{
    return m_statusWord;
}

void InveorInverter::setStatusWord(uint newReadStatusWord)
{
    if (m_statusWord == newReadStatusWord)
        return;
    m_statusWord=newReadStatusWord;
}

uint InveorInverter::targetFrequency()
{
    return m_targetFrequency;
}

void InveorInverter::setTargetFrequency(uint newReadTargetFrequency)
{
    if (m_targetFrequency == newReadTargetFrequency)
        return;
    m_targetFrequency=newReadTargetFrequency;
}

uint InveorInverter::gridVoltage()
{
    return m_gridVoltage;
}

void InveorInverter::setGridVoltage(uint newReadGridVoltage)
{
    if (m_gridVoltage == newReadGridVoltage)
        return;
    m_gridVoltage=newReadGridVoltage;
}

uint InveorInverter::innerTemperature()
{
    return m_innerTemperature;
}

void InveorInverter::setInnerTemperature(uint newReadInnerTemperature)
{
    if (m_innerTemperature == newReadInnerTemperature)
        return;
    m_innerTemperature=newReadInnerTemperature;
}

uint InveorInverter::analogicInput1()
{
    return m_analogicInput1;
}

void InveorInverter::setAnalogicInput1(uint newReadAnalogicInput1)
{
    if (m_analogicInput1 == newReadAnalogicInput1)
        return;
    m_analogicInput1=newReadAnalogicInput1;
}

uint InveorInverter::analogicInput2()
{
    return m_analogicInput2;
}

void InveorInverter::setAnalogicInput2(uint newReadAnalogicInput2)
{
    if (m_analogicInput2 == newReadAnalogicInput2)
        return;
    m_analogicInput2=newReadAnalogicInput2;
}

uint InveorInverter::pidActualValue()
{
    return m_PidActualValue;
}

void InveorInverter::setPidActualValue(uint newreadPidActualValue)
{

    m_PidActualValue=newreadPidActualValue;
}

void InveorInverter::setPidTargetValue(uint newreadPidTargetValue)
{
    if (m_PidTargetValue == newreadPidTargetValue)
        return;
    m_PidTargetValue=newreadPidTargetValue;
}

void InveorInverter::setMaxFrequency(int maxFrequency)
{
    m_maxFrequency=maxFrequency;
}

void InveorInverter::setMinFrequency(int minFrequency)
{
    m_minFrequency=minFrequency;
}

void InveorInverter::setOperatingMode(int operatingMode)
{
    m_operatingMode=operatingMode;
}

void InveorInverter::setSetpointSet(int setpointSet)
{
    m_setpointSet=setpointSet;
}

void InveorInverter::setSetpointApproved(int setpointApproved)
{
    m_setpointApproved=setpointApproved;
}

void InveorInverter::setRotationDirection(int rotationDirection)
{
    m_rotationDirection=rotationDirection;
}

void InveorInverter::setPidP(int pidP)
{
    m_pidP=pidP;
}

void InveorInverter::setPidI(int pidI)
{
    m_pidI=pidI;
}

void InveorInverter::setPidD(int pidD)
{
    m_pidD=pidD;
}

void InveorInverter::setPidInversion(int pidInversion)
{
    m_PidInversion=pidInversion;
}

void InveorInverter::setPidNominalValue(int pidNominalValue)
{
    m_PidNominalValue=pidNominalValue;
}

void InveorInverter::setPidNominalValueMode(int pidNominalValueMode)
{
    m_PidNominalValueMode=pidNominalValueMode;
}

void InveorInverter::setPidNominalValueMax(int pidNominalValueMax)
{
    m_PidNominalValueMax=pidNominalValueMax;
}

void InveorInverter::setPidNominalValueMin(int pidNominalValueMin)
{
    m_PidNominalValueMin=pidNominalValueMin;
}

void InveorInverter::setAnalogicInput1Type(int analogic1InputType)
{
    m_analogicInput1Type=analogic1InputType;
}

void InveorInverter::setAnalogicInput1Min(int analogicInput1Min)
{
    m_analogicInput1Min=analogicInput1Min;

}

void InveorInverter::setAnalogicInput1Max(int analogicInput1Max)
{
    m_analogicInput1Max=analogicInput1Max;
}

void InveorInverter::setAnalogicInput1Function(int analogicInput1Function)
{
    m_analogicInput1Function=analogicInput1Function;
}

void InveorInverter::setAnalogicInput1Unit(int analogicInput1Unit)
{
    m_analogicInput1Unit=analogicInput1Unit;

}

void InveorInverter::setAnalogicInput1UnitMin(int analogicInput1UnitMin)
{
    m_analogicInput1UnitMin=analogicInput1UnitMin;

}

void InveorInverter::setAnalogicInput1UnitMax(int analogicInput1UnitMax)
{
    m_analogicInput1UnitMax=analogicInput1UnitMax;
}

void InveorInverter::setAnalogicInput2Type(int analogicInput2Type)
{
    m_analogicInput2Type=analogicInput2Type;
}

void InveorInverter::setAnalogicInput2Min(int analogicInput2Min)
{
    m_analogicInput2Min=analogicInput2Min;
}

void InveorInverter::setAnalogicInput2Max(int analogicInput2Max)
{
    m_analogicInput2Max=analogicInput2Max;
}

void InveorInverter::setAnalogicInput2Function(int analogicInput2Function)
{
    m_analogicInput2Function=analogicInput2Function;
}

void InveorInverter::setAnalogicInput2Unit(int analogicInput2Unit)
{
    m_analogicInput2Unit=analogicInput2Unit;
}

void InveorInverter::setAnalogicInput2UnitMin(int analogicInput2UnitMin)
{
    m_analogicInput2UnitMin=analogicInput2UnitMin;
}

void InveorInverter::setAnalogicInput2UnitMax(int analogicInput2UnitMax)
{
    m_analogicInput2UnitMax=analogicInput2UnitMax;
}
void InveorInverter::setDeviceId(int deviceId)
{
    m_deviceIndex=deviceId;
}

void InveorInverter::setModbusDeviceIndex(int deviceIndex)
{
    m_deviceIndex=deviceIndex;
}

void InveorInverter::setModbusBaudRate(int baudRate)
{
    m_modbusBaudRate=baudRate;
}

bool InveorInverter::cw0()
{
    return m_cw0;
}

void InveorInverter::setCw0(bool cw0)
{
    m_cw0=cw0;
}
bool InveorInverter::cw1()
{
    return m_cw1;
}

void InveorInverter::setCw1(bool cw1)
{
    m_cw1=cw1;
}
bool InveorInverter::cw2()
{
    return m_cw2;
}

void InveorInverter::setCw2(bool cw2)
{
    m_cw2=cw2;
}
bool InveorInverter::cw3()
{
    return m_cw3;
}

void InveorInverter::setCw3(bool cw3)
{
    m_cw3=cw3;
}
bool InveorInverter::cw4()
{
    return m_cw4;
}

void InveorInverter::setCw4(bool cw4)
{
    m_cw4=cw4;
}
bool InveorInverter::cw5()
{
    return m_cw5;
}

void InveorInverter::setCw5(bool cw5)
{
    m_cw5=cw5;
}
bool InveorInverter::cw6()
{
    return m_cw6;
}

void InveorInverter::setCw6(bool cw6)
{
    m_cw6=cw6;
}
bool InveorInverter::cw7()
{
    return m_cw7;
}

void InveorInverter::setCw7(bool cw7)
{
    m_cw7=cw7;
}
bool InveorInverter::cw8()
{
    return m_cw8;
}

void InveorInverter::setCw8(bool cw8)
{
    m_cw8=cw8;
}
bool InveorInverter::cw9()
{
    return m_cw9;
}

void InveorInverter::setCw9(bool cw9)
{
    m_cw9=cw9;
}
bool InveorInverter::cw10()
{
    return m_cw10;
}

void InveorInverter::setCw10(bool cw10)
{
    m_cw10=cw10;
}
bool InveorInverter::cw11()
{
    return m_cw11;
}

void InveorInverter::setCw11(bool cw11)
{
    m_cw11=cw11;
}
bool InveorInverter::cw12()
{
    return m_cw12;
}

void InveorInverter::setCw12(bool cw12)
{
    m_cw12=cw12;
}
bool InveorInverter::cw13()
{
    return m_cw13;
}

void InveorInverter::setCw13(bool cw13)
{
    m_cw13=cw13;
}
bool InveorInverter::cw14()
{
    return m_cw14;
}

void InveorInverter::setCw14(bool cw14)
{
    m_cw14=cw14;
}
bool InveorInverter::cw15()
{
    return m_cw15;
}

void InveorInverter::setCw15(bool cw15)
{
    m_cw15=cw15;
}
bool InveorInverter::sw0()
{
    return m_sw0;
}

void InveorInverter::setSw0(bool sw0)
{
    m_sw0=sw0;
}
bool InveorInverter::sw1()
{
    return m_sw1;
}

void InveorInverter::setSw1(bool sw1)
{
    m_sw1=sw1;
}
bool InveorInverter::sw2()
{
    return m_sw2;
}

void InveorInverter::setSw2(bool sw2)
{
    m_sw2=sw2;
}
bool InveorInverter::sw3()
{
    return m_sw3;
}

void InveorInverter::setSw3(bool sw3)
{
    m_sw3=sw3;
}
bool InveorInverter::sw4()
{
    return m_sw4;
}

void InveorInverter::setSw4(bool sw4)
{
    m_sw4=sw4;
}
bool InveorInverter::sw5()
{
    return m_sw5;
}

void InveorInverter::setSw5(bool sw5)
{
    m_sw5=sw5;
}
bool InveorInverter::sw6()
{
    return m_sw6;
}

void InveorInverter::setSw6(bool sw6)
{
    m_sw6=sw6;
}
bool InveorInverter::sw7()
{
    return m_sw7;
}

void InveorInverter::setSw7(bool sw7)
{
    m_sw7=sw7;
}
bool InveorInverter::sw8()
{
    return m_sw8;
}

void InveorInverter::setSw8(bool sw8)
{
    m_sw8=sw8;
}
bool InveorInverter::sw9()
{
    return m_sw9;
}

void InveorInverter::setSw9(bool sw9)
{
    m_sw9=sw9;
}
bool InveorInverter::sw10()
{
    return m_sw10;
}

void InveorInverter::setSw10(bool sw10)
{
    m_sw10=sw10;
}
bool InveorInverter::sw11()
{
    return m_sw11;
}

void InveorInverter::setSw11(bool sw11)
{
    m_sw11=sw11;
}
bool InveorInverter::sw12()
{
    return m_sw12;
}
void InveorInverter::setSw12(bool sw12)
{
    m_sw12=sw12;
}

bool InveorInverter::sw13()
{
    return m_sw13;
}
void InveorInverter::setSw13(bool sw13)
{
    m_sw13=sw13;
}

bool InveorInverter::sw14()
{
    return m_sw14;
}
void InveorInverter::setSw14(bool sw14)
{
    m_sw14=sw14;
}

bool InveorInverter::sw15()
{
    return m_sw15;
}
void InveorInverter::setSw15(bool sw15)
{
    m_sw15=sw15;
}

BaudRate InveorInverter::baudRate()
{
    return m_baudRate;
}
Parity InveorInverter::parity()
{
    return m_parity;
}
DataBits InveorInverter::dataBits()
{
    return m_dataBits;
}
StopBits InveorInverter::stopBits()
{
    return m_stopBits;
}
QString InveorInverter::serialPort()
{
    return m_serialPort;
}
QString InveorInverter::tcpPort()
{
    return m_tcpPort;
}
QString InveorInverter::tcpIpAddress()
{
    return m_tcpIpAddress;
}
int InveorInverter::numberRetries()
{
    return m_numRetries;
}
int InveorInverter::timeout()
{
    return m_timeout;
}
int InveorInverter::deviceId(){
    return m_deviceIndex;
}
uint InveorInverter::maxFrequency(){
    return m_maxFrequency;
}
uint InveorInverter::minFrequency(){
    return m_minFrequency;
}
uint InveorInverter::operatingMode(){
    return m_operatingMode;
}
uint InveorInverter::setpointSet(){
    return m_setpointSet;
}
uint InveorInverter::setpointApproved(){
    return m_setpointApproved;
}
uint InveorInverter::rotationDirection(){
    return m_rotationDirection;
}
uint InveorInverter::pidP(){
    return m_pidP;
}
uint InveorInverter::pidI(){
    return m_pidI;
}
uint InveorInverter::pidD(){
    return m_pidD;
}
uint InveorInverter::pidInversion(){
    return m_PidInversion;
}
uint InveorInverter::pidNominalValue(){
    return m_PidNominalValue;
}
uint InveorInverter::pidNominalValueMode(){
    return m_PidNominalValueMode;
}
uint InveorInverter::pidNominalValueMax(){
    return m_PidNominalValueMax;
}
uint InveorInverter::pidNominalValueMin(){
    return m_PidNominalValueMin;
}
uint InveorInverter::analogicInput1Type(){
    return m_analogicInput1Type;
}
uint InveorInverter::analogicInput1Min(){
    return m_analogicInput1Min;
}
uint InveorInverter::analogicInput1Max(){
    return m_analogicInput1Max;
}
uint InveorInverter::analogicInput1Function(){
    return m_analogicInput1Function;
}
uint InveorInverter::analogicInput1Unit(){
    return m_analogicInput1Unit;
}
uint InveorInverter::analogicInput1UnitMin(){
    return m_analogicInput1UnitMin;
}
uint InveorInverter::analogicInput1UnitMax(){
    return m_analogicInput1UnitMax;
}
uint InveorInverter::analogicInput2Type(){
    return m_analogicInput2Type;
}
uint InveorInverter::analogicInput2Min(){
    return m_analogicInput2Min;
}
uint InveorInverter::analogicInput2Max(){
    return m_analogicInput2Max;
}
uint InveorInverter::analogicInput2Function(){
    return m_analogicInput2Function;
}
uint InveorInverter::analogicInput2Unit(){
    return m_analogicInput2Unit;
}
uint InveorInverter::analogicInput2UnitMin(){
    return m_analogicInput2UnitMin;
}
uint InveorInverter::analogicInput2UnitMax(){
    return m_analogicInput2UnitMax;
}
uint InveorInverter::modbusDeviceIndex(){
    return m_deviceIndex;
}
uint InveorInverter::modbusBaudRate(){
    return m_modbusBaudRate;
}


