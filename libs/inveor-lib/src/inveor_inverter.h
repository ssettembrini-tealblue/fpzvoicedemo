
#ifndef INVEORLIBRARY_H
#define INVEORLIBRARY_H
#include <QModbusDataUnit>
#include<QSerialPort>
#include<QModbusReply>
#include"writeregistermodel.h"
QT_BEGIN_NAMESPACE
class QModbusClient;
QT_END_NAMESPACE
class WriteRegisterModel;

enum class status{
    Disconnected=0,
    Connected=1,
    ForcedDisconnect=2,
    Connecting=3,
    Unknown=4
};
enum class modbusError{
    Read=0,
    ProtocolRead=1,
    GenericRead=2,
    Write=3,
    ProtocolWrite=4,
    GenericWrite=5
};


enum class BaudRate{
    Baud1200=1200,
    Baud2400=2400,
    Baud4800=4800,
    Baud9600=9600,
    Baud19200=19200,
    Baud38400=38400,
    Baud57600=57600,
    Baud115200=115200,
    UnknownBaud=-1
};
enum class StopBits{
    OneStop=1,
    OneAndHalfStop=3,
    TwoStop=2,
    UnknownStopBits=-1
};

enum class DataBits{
    Data5=5,
    Data6=6,
    Data7=7,
    Data8=8,
    UnknownDataBits=-1
};

enum class Parity{
    NoParity=0,
    EvenParity=2,
    OddParity=3,
    SpaceParity=4,
    MarkParity=5,
    UnknownParity=-1
};

class InveorInverter
{
public:
    InveorInverter();
    WriteRegisterModel* writeRegisterModel() const;
    BaudRate baudRate();
    Parity parity();
    DataBits dataBits();
    StopBits stopBits();
    QString serialPort();
    QString tcpPort();
    QString tcpIpAddress();
    int timeout();
    int numberRetries();
    int deviceId();
    QString statusMsg();
    QString status();
    bool isConnected();
    void connect();
    void disconnect();

    void setupConnection(int id, QString port, BaudRate baud, Parity par, DataBits data, StopBits stop);

    uint maxFrequency();
    uint minFrequency();
    uint operatingMode();
    uint setpointSet();
    uint setpointApproved();
    uint rotationDirection();
    uint pidP();
    uint pidI();
    uint pidD();
    uint pidInversion();
    uint pidNominalValue();
    uint pidNominalValueMode();
    uint pidNominalValueMax();
    uint pidNominalValueMin();
    uint analogicInput1Type();
    uint analogicInput1Min();
    uint analogicInput1Max();
    uint analogicInput1Function();
    uint analogicInput1Unit();
    uint analogicInput1UnitMin();
    uint analogicInput1UnitMax();
    uint analogicInput2Type();
    uint analogicInput2Min();
    uint analogicInput2Max();
    uint analogicInput2Function();
    uint analogicInput2Unit();
    uint analogicInput2UnitMin();
    uint analogicInput2UnitMax();
    uint modbusDeviceIndex();
    uint modbusBaudRate();
    uint statusWord();
    uint pidActualValue();
    uint actualFrequency();
    uint motorCurrent();
    uint motorVoltage();
    uint targetFrequency();
    uint gridVoltage();
    uint innerTemperature();
    uint analogicInput1();
    uint analogicInput2();
    uint pidTargetValue();

    bool readOutputs();
    bool readMaxFrequency();
    bool readMinFrequency();
    bool readOperatingMode();
    bool readSetpointSet();
    bool readSetpointApproved();
    bool readRotationDirection();
    bool readPidP();
    bool readPidI();
    bool readPidD();
    bool readPidActualValue();
    bool readPidInversion();
    bool readPidNominalValue();
    bool readPidNominalValueMode();
    bool readPidNominalValueMax();
    bool readPidNominalValueMin();
    bool readAnalogicInput1Type();
    bool readAnalogicInput1Min();
    bool readAnalogicInput1Max();
    bool readAnalogicInput1Function();
    bool readAnalogicInput1Unit();
    bool readAnalogicInput1UnitMin();
    bool readAnalogicInput1UnitMax();
    bool readAnalogicInput2Type();
    bool readAnalogicInput2Min();
    bool readAnalogicInput2Max();
    bool readAnalogicInput2Function();
    bool readAnalogicInput2Unit();
    bool readAnalogicInput2UnitMin();
    bool readAnalogicInput2UnitMax();
    bool readModbusDeviceIndex();
    bool readModbusBaudRate();
    bool readStatusWord();
    bool readActualFrequency();
    bool readMotorVoltage();
    bool readMotorCurrent();
    bool readTargetFrequency();
    bool readGridVoltage();
    bool readInnerTemperature();
    bool readAnalogicInput1();
    bool readAnalogicInput2();
    bool readPidTargetValue();

    bool writeMaxFrequency(double maxFrequency);
    bool writeMinFrequency(double minFrequency);
    bool writeOperatingMode(double operatingMode);
    bool writeSetpointSet(double setpointSet);
    bool writeSetpointApproved(double setpointApproved);
    bool writeRotationDirection(double rotationDirection);
    bool writePidP(double pidP);
    bool writePidI(double pidI);
    bool writePidD(double pidD);
    bool writePidActualValue(double pidActualValue);
    bool writePidInversion(double pidInversion);
    bool writePidNominalValue(double pidNominalValue);
    bool writePidNominalValueMode(double pidNominalValueMode);
    bool writePidNominalValueMax(double pidNominalValueMax);
    bool writePidNominalValueMin(double pidNominalValueMin);
    bool writeAnalogicInput1Type(double analogic1InputType);
    bool writeAnalogicInput1Min(double analogicInput1Min);
    bool writeAnalogicInput1Max(double analogicInput1Max);
    bool writeAnalogicInput1Function(double analogicInput1Function);
    bool writeAnalogicInput1Unit(double analogicInput1Unit);
    bool writeAnalogicInput1UnitMin(double analogicInput1UnitMin);
    bool writeAnalogicInput1UnitMax(double analogicInput1UnitMax);
    bool writeAnalogicInput2Type(double analogicInput2Type);
    bool writeAnalogicInput2Min(double analogicInput2Min);
    bool writeAnalogicInput2Max(double analogicInput2Max);
    bool writeAnalogicInput2Function(double analogicInput2Function);
    bool writeAnalogicInput2Unit(double analogicInput2Unit);
    bool writeAnalogicInput2UnitMin(double analogicInput2UnitMin);
    bool writeAnalogicInput2UnitMax(double analogicInput2UnitMax);
    bool writeModbusDeviceIndex(double deviceIndex);
    bool writeModbusBaudRate(double baudRate);
    bool writeDO1function(int function);
    bool writeDO1on(int onValue);
    bool writeDO1off(int offValue);
    bool writeDO2function(int function);
    bool writeDO2on(int onValue);
    bool writeDO2off(int offValue);

    bool writeDO2(bool value);
    bool writeNominalFrequency(uint value);
    bool writeControlWord(uint newControlWord);


private:

    void setBaud(BaudRate baudRate);
    void setParity(Parity parity);
    void setDataBits(DataBits dataBits);
    void setStopBits(StopBits stopBits);
    void setSerialPort(QString serialPort);
    void setTcpPort(QString tcpPort);
    void setTcpAddress(QString ipAddress);
    void setNumRetries(int numRetries);
    void setTimeout(int timeout);
    void setDeviceId(int deviceId);

    void setStatus(const QString& commStatus="");

    void sendError(modbusError error, QModbusReply* reply);

    QModbusDataUnit readRequest(int registerType,quint16 numberEntries,int address) const;
    QModbusDataUnit writeRequest(int registerType,quint16 numberEntries, int address) const;

    void setMotorVoltage(uint newReadVoltage);
    void setMotorCurrent(uint newReadPower);
    void setReadActualFrequency(uint newReadFrequency);
    void setStatusWord(uint newReadStatusWord);
    void setTargetFrequency(uint newReadTargetFrequency);
    void setGridVoltage(uint newReadGridVoltage);
    void setInnerTemperature(uint newReadInnerTemperature);
    void setAnalogicInput1(uint newReadAnalogicInput1);
    void setAnalogicInput2(uint newReadAnalogicInput2);
    void setPidActualValue(uint newReadPIDActualValue);
    void setPidTargetValue(uint newReadPIDTargetValue);

    void setMaxFrequency(int maxFrequency);
    void setMinFrequency(int minFrequency);
    void setOperatingMode(int operatingMode);
    void setSetpointSet(int setpointSet);
    void setSetpointApproved(int setpointApproved);
    void setRotationDirection(int rotationDirection);
    void setPidP(int pidP);
    void setPidI(int pidI);
    void setPidD(int pidD);
    void setPidInversion(int pidInversion);
    void setPidNominalValue(int pidNominalValue);
    void setPidNominalValueMode(int pidNominalValueMode);
    void setPidNominalValueMax(int pidNominalValueMax);
    void setPidNominalValueMin(int pidNominalValueMin);
    void setAnalogicInput1Type(int analogic1InputType);
    void setAnalogicInput1Min(int analogicInput1Min);
    void setAnalogicInput1Max(int analogicInput1Max);
    void setAnalogicInput1Function(int analogicInput1Function);
    void setAnalogicInput1Unit(int analogicInput1Unit);
    void setAnalogicInput1UnitMin(int analogicInput1UnitMin);
    void setAnalogicInput1UnitMax(int analogicInput1UnitMax);
    void setAnalogicInput2Type(int analogic1InputType);
    void setAnalogicInput2Min(int analogicInput2Min);
    void setAnalogicInput2Max(int analogicInput2Max);
    void setAnalogicInput2Function(int analogicInput2Function);
    void setAnalogicInput2Unit(int analogicInput2Unit);
    void setAnalogicInput2UnitMin(int analogicInput2UnitMin);
    void setAnalogicInput2UnitMax(int analogicInput2UnitMax);
    void setModbusDeviceIndex(int deviceIndex);
    void setModbusBaudRate(int baudRate);



    bool cw0();
    void setCw0(bool cw0);
    bool cw1();
    void setCw1(bool cw1);
    bool cw2();
    void setCw2(bool cw2);
    bool cw3();
    void setCw3(bool cw3);
    bool cw4();
    void setCw4(bool cw4);
    bool cw5();
    void setCw5(bool cw5);
    bool cw6();
    void setCw6(bool cw6);
    bool cw7();
    void setCw7(bool cw7);
    bool cw8();
    void setCw8(bool cw8);
    bool cw9();
    void setCw9(bool cw9);
    bool cw10();
    void setCw10(bool cw10);
    bool cw11();
    void setCw11(bool cw11);
    bool cw12();
    void setCw12(bool cw12);
    bool cw13();
    void setCw13(bool cw13);
    bool cw14();
    void setCw14(bool cw14);
    bool cw15();
    void setCw15(bool cw15);


    bool sw0();
    void setSw0(bool sw0);
    bool sw1();
    void setSw1(bool sw1);
    bool sw2();
    void setSw2(bool sw2);
    bool sw3();
    void setSw3(bool sw3);
    bool sw4();
    void setSw4(bool sw4);
    bool sw5();
    void setSw5(bool sw5);
    bool sw6();
    void setSw6(bool sw6);
    bool sw7();
    void setSw7(bool sw7);
    bool sw8();
    void setSw8(bool sw8);
    bool sw9();
    void setSw9(bool sw9);
    bool sw10();
    void setSw10(bool sw10);
    bool sw11();
    void setSw11(bool sw11);
    bool sw12();
    void setSw12(bool sw12);
    bool sw13();
    void setSw13(bool sw13);
    bool sw14();
    void setSw14(bool sw14);
    bool sw15();
    void setSw15(bool sw15);

    bool digOut1();
    void setDigOut1(bool digout1);
    bool digOut2();
    void setDigOut2(bool digout2);
    bool relay1();
    void setRelay1(bool relay1);
    bool relay2();
    void setRelay2(bool relay2);
    bool virtOut1();
    void setVirtOut1(bool virtout1);


    QString m_commStatus = "";
    enum status m_connStatus=status::Disconnected;

    QModbusClient* m_modbusDevice = nullptr;
    int m_deviceIndex{0};

    BaudRate m_baudRate{BaudRate::Baud38400};
    Parity m_parity{Parity::NoParity};
    DataBits m_dataBits{DataBits::Data8};
    StopBits m_stopBits{StopBits::OneStop};
    QString m_serialPort;
    QString m_tcpPort;
    QString m_tcpIpAddress;
    int m_numRetries{20};
    int m_timeout{20};

    bool m_digOut1{0};
    bool m_digOut2{0};
    bool m_relay1{0};
    bool m_relay2{0};
    bool m_virtOut1{0};

    uint m_maxFrequency{0};
    uint m_minFrequency{0};
    uint m_operatingMode{0};
    uint m_setpointSet{0};
    uint m_setpointApproved{0};
    uint m_rotationDirection{0};
    uint m_pidP{0};
    uint m_pidI{0};
    uint m_pidD{0};
    uint m_PidInversion{0};
    uint m_PidNominalValue{0};
    uint m_PidNominalValueMode{0};
    uint m_PidNominalValueMax{0};
    uint m_PidNominalValueMin{0};
    uint m_analogicInput1Type{1};
    uint m_analogicInput1Min{0};
    uint m_analogicInput1Max{0};
    uint m_analogicInput1Function{0};
    uint m_analogicInput1Unit{0};
    uint m_analogicInput1UnitMin{0};
    uint m_analogicInput1UnitMax{0};
    uint m_analogicInput2Type{1};
    uint m_analogicInput2Min{0};
    uint m_analogicInput2Max{0};
    uint m_analogicInput2Function{0};
    uint m_analogicInput2Unit{0};
    uint m_analogicInput2UnitMin{0};
    uint m_analogicInput2UnitMax{0};
    uint m_modbusBaudRate{0};

    uint m_motorVoltage{0};
    uint m_motorCurrent{0};
    uint m_actualFrequency{0};
    uint m_statusWord{0};
    uint m_targetFrequency{0};
    uint m_gridVoltage{0};
    uint m_innerTemperature{0};
    uint m_analogicInput1{0};
    uint m_analogicInput2{0};
    uint m_pidActualValue{0};
    uint m_PidTargetValue{0};

    bool m_cw0{0};
    bool m_cw1{0};
    bool m_cw2{0};
    bool m_cw3{0};
    bool m_cw4{0};
    bool m_cw5{0};
    bool m_cw6{0};
    bool m_cw7{0};
    bool m_cw8{0};
    bool m_cw9{0};
    bool m_cw10{0};
    bool m_cw11{0};
    bool m_cw12{0};
    bool m_cw13{0};
    bool m_cw14{0};
    bool m_cw15{0};

    bool m_sw0{0};
    bool m_sw1{0};
    bool m_sw2{0};
    bool m_sw3{0};
    bool m_sw4{0};
    bool m_sw5{0};
    bool m_sw6{0};
    bool m_sw7{0};
    bool m_sw8{0};
    bool m_sw9{0};
    bool m_sw10{0};
    bool m_sw11{0};
    bool m_sw12{0};
    bool m_sw13{0};
    bool m_sw14{0};
    bool m_sw15{0};

    WriteRegisterModel* m_writeRegisterModel;

};

#endif // INVEORLIBRARY_H
