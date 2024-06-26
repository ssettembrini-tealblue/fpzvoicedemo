#ifndef CLIENT_ACTIONS_H
#define CLIENT_ACTIONS_H
#include <QObject>

class ClientActions : public QObject {
    Q_OBJECT
public:
    explicit ClientActions(QObject *parent = nullptr) : QObject(parent) {};
signals:
    void selectNewDevice(const QString& deviceName);
    void selectKnownDevice(const QString& deviceName);
    void startBtDeviceDiscovery(const QString& deviceName, const QString& serviceUuid);
    //void sendFrequency(int value);
    void stopBtDeviceDiscovery();

    void triggerWakeWord();
    void switchLanguage();
    void checkLanguage();

    void connectDevice();
    //void writeParameters(int value);
    //void readParameters();
    //void setAddress(int address);
    void disconnectDevice();

    void quitApp();
    void startDO2();
    void stopDO2();
    void increaseNominalFrequency(uint step);
    void decreaseNominalFrequency(uint step);
    void writeNominalFrequency(uint nominalFrequency);
    void stopBlower();
    void startBlower();
    void writeMaxFrequency(uint maxFrequency);
    void writeMinFrequency(uint minFrequency);
    void writeOperatingMode(uint operatingMode);
    void writeSettingSetpoint(uint settingSetpoint);
    void writeApprovalSetpoint(uint approvalSetpoint);
    void writePidActualValue(uint pidActualValue);
    void writePidNominalValue(uint pidNominalValue);
    void writeAnalogicInput1Type(uint analogicInput1Type);
    void writeAnalogicInput1Min(uint analogicInput1Min);
    void writeAnalogicInput1Max(uint analogicInput1Max);
    void writeDO1function(uint function);
    void writeDO1on(uint onValue);
    void writeDO1off(uint offValue);
    void writeDO2function(uint function);
    void writeDO2on(uint onValue);
    void writeDO2off(uint offValue);

};


#endif // CLIENT_ACTIONS_H
