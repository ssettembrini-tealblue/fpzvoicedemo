#ifndef MOCK_INVEOR_STORE_H
#define MOCK_INVEOR_STORE_H
#include "client_actions.h"
#include "../libs/inveor-lib/src/inveor_inverter.h"
#include <QModbusDataUnit>
#include <QTimer>
QT_BEGIN_NAMESPACE
class QModbusClient;
class QModbusReply;
QT_END_NAMESPACE

class MockInveorStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString connStatus READ connStatus NOTIFY connStatusChanged)
    Q_PROPERTY(QString msgStatus READ msgStatus NOTIFY commStatusChanged)
    Q_PROPERTY(uint statusWord READ statusWord NOTIFY statusWordChanged)
    Q_PROPERTY(uint actualFrequency READ actualFrequency NOTIFY actualFrequencyChanged)
    Q_PROPERTY(uint motorVoltage READ motorVoltage NOTIFY motorVoltageChanged)
    Q_PROPERTY(uint motorCurrent READ motorCurrent NOTIFY motorCurrentChanged)
    Q_PROPERTY(uint gridVoltage READ gridVoltage NOTIFY gridVoltageChanged)
    Q_PROPERTY(uint innerTemp READ innerTemp NOTIFY innerTempChanged)
    Q_PROPERTY(uint minFrequency READ minFrequency NOTIFY minFrequencyChanged)
    Q_PROPERTY(uint maxFrequency READ maxFrequency NOTIFY maxFrequencyChanged)
    Q_PROPERTY(uint nomFreq READ nomFreq NOTIFY nomFreqChanged)

public:
    explicit MockInveorStore(ClientActions* clientActions,QObject *parent = nullptr);
    QTimer *m_timer;

    void readValues();

    void setNominalFrequency(uint nominalFrequency);

    QString connStatus();
    QString msgStatus();
    uint minFrequency() ;
    uint maxFrequency();
    uint operatingMode();
    uint settingSetpoint();
    uint approvalSetpoint();
    uint pidActualValue();
    uint statusWord();
    uint actualFrequency();
    uint motorVoltage();
    uint motorCurrent();
    uint gridVoltage();
    uint innerTemp();
    uint pidNominalValue();
    uint analogicInput1Type();
    uint analogicInput1Min();
    uint analogicInput1Max();    
    uint nomFreq() const;

signals:
    void connStatusChanged();
    void commStatusChanged();
    void nominalFrequencyChanged(uint newNominalFrequency);
    void minFrequencyChanged(uint newMinFrequency);
    void maxFrequencyChanged(uint newMaxFrequency);
    void operatingModeChanged(uint newOperatingMode);
    void settingSetpointChanged(uint newSettingSetpoint);
    void approvalSetpointChanged(uint newApprovalSetpointChanged);
    void pidActualValueChanged(uint newPIDActualValue);
    void statusWordChanged();
    void actualFrequencyChanged();
    void motorVoltageChanged();
    void motorCurrentChanged();
    void gridVoltageChanged();
    void innerTempChanged();
    void pidNominalValueChanged(uint newPIDNominalValue);
    void analogicInput1TypeChanged(uint newAnalogicInput1Type);
    void analogicInput1MinChanged(uint newAnalogicInput1Min);
    void analogicInput1MaxChanged(uint newAnalogicInput1Max);
    void nomFreqChanged(uint nomFreq);

private:
    ClientActions* m_clientActions;
    InveorInverter m_inveor_inverter;
    QString m_msgStatus;
    uint m_minFrequency {0};
    uint m_maxFrequency {0};
    uint m_operatingMode {0};
    uint m_settingSetpoint {0};
    uint m_approvalSetpoint {0};
    uint m_pidActualValue {0};
    uint m_statusWord {0};
    uint m_actualFrequency {0};
    uint m_motorVoltage {0};
    uint m_motorCurrent {0};
    uint m_gridVoltage {0};
    uint m_innerTemp {0};
    uint m_pidNominalValue {0};
    uint m_analogicInput1Type {0};
    uint m_analogicInput1Min {0};
    uint m_analogicInput1Max {0};
    uint m_nomFreq {0};
};

#endif // MOCK_INVEOR_STORE_H
