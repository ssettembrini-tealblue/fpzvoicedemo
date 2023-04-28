#ifndef BLUETOOTH_SERVICES_MODEL_H
#define BLUETOOTH_SERVICES_MODEL_H

#include <QAbstractListModel>
#include <QSettings>
#include <QSharedPointer>
#include <btscanner.h>

class BluetoothServicesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    static constexpr char const* dataRoleName = "data";
    explicit BluetoothServicesModel( QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        m_roleNames.insert(Qt::UserRole + 1, dataRoleName);
    }

    ~BluetoothServicesModel() override;

    void clear() {
        updateData({});
    }

    void addRow(const QVariantMap &r)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());

        m_data.append(r);

        endInsertRows();
    }

    void removeRow(int idx)
    {
        beginRemoveRows(QModelIndex(), idx, idx);
        QVariant  tmp = m_data.takeAt(idx);
        endRemoveRows();
    }

    void updateData(const QVariantList &data)
    {
        beginResetModel();
        m_data = data;
        endResetModel();
    }

public: // from QAbstractListModel

    int rowCount(const QModelIndex &modelIndex = QModelIndex()) const override
    {
        Q_UNUSED(modelIndex)
        return m_data.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::UserRole + 1) const override
    {
        const int row = index.row();
        if (!index.isValid() || row < 0 || row >= m_data.size() || role != Qt::UserRole + 1)
            return QVariant();

        return QVariant::fromValue(m_data.at(row));
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return m_roleNames;
    }

signals:
    void deviceSelected(int index);

protected:
    QHash<int, QByteArray> m_roleNames;
    QVariantList m_data;
};


// Stores QBluetoothServiceInfo separately, m_data holds usual name, idx, uuid.
class SavedBluetoothServicesModel : public BluetoothServicesModel
{
    Q_OBJECT
public:
    explicit SavedBluetoothServicesModel(QObject *parent = nullptr): BluetoothServicesModel(parent)
    {
        fromSettings();
    }

    ~SavedBluetoothServicesModel() override;

    void sync() {
        toSettings();
        settings.sync();
    }

    int serviceIdx(const QString &deviceName, const QString &servUuid) {
        int idx = 0;
        for (const auto &service: m_data) {
            const auto &map = service.toMap();
            Q_ASSERT(map.contains("deviceName"));
            Q_ASSERT(map.contains("servUuid"));
            if (map.value("deviceName").toString() == deviceName
                && map.value("servUuid").toString() == servUuid)
                return idx;
            ++idx;
        }
        return -1;
    }


    void addService(const QBluetoothServiceInfo &info) {
        auto devUuid = info.device().deviceUuid().toString(QUuid::WithBraces);
        auto servUuid = info.serviceUuid().toString(QUuid::WithoutBraces);
        auto deviceName = info.device().name();
        if (serviceIdx(deviceName, servUuid) >= 0)
            return;
        m_infos.append(info);
        QVariantMap record;
        record["deviceName"] = deviceName;
        record["idx"] = m_infos.size();
        record["devUuid"] = devUuid;
        record["servUuid"] = servUuid;
//        qDebug() << record;
        addRow(record);
    }

    QBluetoothServiceInfo serviceInfo(const QString &deviceName, const QString &servUuid)
    {
        int idx = serviceIdx(deviceName, servUuid);
        if (idx < 0)
            return {};
        return m_infos.at(idx);
    }

    QBluetoothServiceInfo serviceInfo(unsigned int idx=0)
    {
        return m_infos.at(idx);
    }

protected:
    void fromSettings()
    {
        QVariantList serializedBSIs = settings.value("saved_services").toList();
        for (int i = 0; i < serializedBSIs.size(); ++i) {
            const auto &sbsi = serializedBSIs.at(i).toString();
            QBluetoothServiceInfo info = BtScanner::deserializeBluetoothServiceInfo(sbsi);
            addService(info);
        }
    }
    void toSettings()
    {
        QVariantList serializedBSIs;
        for (const auto &bsi: m_infos) {
//            qDebug() << "Serializing "<< bsi.serviceUuid().toString();
            serializedBSIs.append(BtScanner::serializeBluetoothServiceInfo(bsi));
        }
//        qDebug() << "toSettings";
//        qDebug() << serializedBSIs;
        settings.setValue("saved_services", serializedBSIs);
    }


    QSettings settings;
    QList<QBluetoothServiceInfo> m_infos;
};

#endif // BLUETOOTH_SERVICES_MODEL_H
