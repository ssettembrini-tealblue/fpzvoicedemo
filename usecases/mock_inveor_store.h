#ifndef MOCK_INVEOR_STORE_H
#define MOCK_INVEOR_STORE_H
#include "client_actions.h"
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
    Q_PROPERTY(uint actualFrequency READ actualFrequency NOTIFY actualFrequencyChanged)
    Q_PROPERTY(uint motorVoltage WRITE setMotorVoltage READ motorVoltage NOTIFY motorVoltageChanged)
    Q_PROPERTY(uint motorCurrent WRITE setMotorCurrent READ motorCurrent NOTIFY motorCurrentChanged)
    Q_PROPERTY(uint minFrequency READ minFrequency NOTIFY minFrequencyChanged)
    Q_PROPERTY(uint maxFrequency READ maxFrequency NOTIFY maxFrequencyChanged)
    Q_PROPERTY(uint nomFreq WRITE setNominalFrequency READ nomFreq NOTIFY nominalFrequencyChanged)

public:
    explicit MockInveorStore(ClientActions* clientActions,QObject *parent = nullptr);
    QTimer *m_timer;

    void setNominalFrequency(uint nominalFrequency);
    void setMotorVoltage(uint newMotorVoltage);
    void setMotorCurrent(uint newMotorCurrent);

    QString connStatus();
    QString msgStatus();
    uint minFrequency() ;
    uint maxFrequency();
    uint actualFrequency();
    uint motorVoltage();
    uint motorCurrent();
    uint nomFreq();

signals:
    void connStatusChanged();
    void commStatusChanged();
    void nominalFrequencyChanged(uint newNominalFrequency);
    void minFrequencyChanged(uint newMinFrequency);
    void maxFrequencyChanged(uint newMaxFrequency);
    void actualFrequencyChanged();
    void motorVoltageChanged(uint motVol);
    void motorCurrentChanged(uint motCur);

private:
    ClientActions* m_clientActions;
    QString m_msgStatus;
    uint m_minFrequency {0};
    uint m_maxFrequency {50};
    uint m_actualFrequency {0};
    uint m_motorVoltage {0};
    uint m_motorCurrent {0};
    uint m_nomFreq {0};
};

#endif // MOCK_INVEOR_STORE_H
