#ifndef BTSCANNER_H
#define BTSCANNER_H

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QObject>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothsocket.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QUrl>
#include <QDebug>
#include <QLatin1String>
#include <QBluetoothAddress>
#include <qbluetoothuuid.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothdevicediscoveryagent.h>
#include "btreceiver.h"

namespace  {
static constexpr const quint128 null128{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
bool operator== (const quint128 &l, const quint128 &r)
{
    for (int i = 0; i < 16; ++i)
        if (l.data[i] != r.data[i])
            return false;
    return true;
}
bool operator!= (const quint128 &l, const quint128 &r)
{
    return !operator==(l, r);
}
quint128 operator& (const quint128 &l, const quint128 &r)
{
    quint128 res;
    for (int i = 0; i < 16; ++i) {
        res.data[i] = l.data[i] & r.data[i];
    }
    return res;
}
quint128 operator& (const quint128 &l, const quint16 &r)
{
    quint128 res;
    for (int i = 0; i < 16; ++i) {
        res.data[i] = 0;
        if (r & (1<<i))
            res.data[i] = l.data[i];
    }
    return res;
}
quint128 operator| (const quint128 &l, const quint128 &r)
{
    quint128 res;
    for (int i = 0; i < 16; ++i) {
        res.data[i] = l.data[i] | r.data[i];
    }
    return res;
}
bool isNull(const quint128 &o) {
    return o == null128;
}
QVariant parseAttribute(QString attr, int level = 0)
{
    char seps[6] = "#?|£";

    if (attr.size() <= 0)
        return QVariant();

    if (attr[0] == '(') {
        //This is a sequence
        QBluetoothServiceInfo::Sequence seq;
        // Strip front and back
        QString content = attr.right(attr.size()-1);
        content.chop(1);
        QStringList items = content.split(seps[level]);
        for (int i=0; i < items.length(); i++  ) {
            QVariant element = parseAttribute(items[i], level+1);
            seq.append(element);
        }
        return QVariant::fromValue(seq);
    } else {
        // Base case, split the first .
        QString valtype = attr.left(attr.indexOf("."));
        QString val = attr.right(attr.size()-attr.indexOf(".")-1);

        if (valtype == "int") {
            int res = val.toInt();
            return QVariant::fromValue(res);
        } else if (valtype == "string") {
            return QVariant::fromValue(val);
        } else if (valtype == "bool") {
            bool res = val.toInt();
            return QVariant::fromValue(res);
        } else if (valtype == "url") {
            QUrl res = QUrl(val);
            return QVariant::fromValue(res);
        } else if (valtype == "uuid16") {
            return QVariant::fromValue(QBluetoothUuid(quint16(val.toUInt())));
        } else if (valtype == "uuid32") {
            return QVariant::fromValue(QBluetoothUuid(quint32(val.toUInt())));
        } else if (valtype == "uuid128") {
            return QVariant::fromValue(QBluetoothUuid(val));
        } else {
            return QVariant();
        }
    }
}
void dumpAttributeVariant(const QVariant &var, QString *out) //, const QString indent)
{
    switch (var.type()) {
    case QMetaType::Void:
        //            qDebug("%sEmpty", indent.toLocal8Bit().constData());
        break;
    case QMetaType::UChar:
        //            qDebug("%suchar %u", indent.toLocal8Bit().constData(), var.toUInt());
        (*out) += ("int." +QString::number(var.toInt()));
        break;
    case QMetaType::UShort:
        //            qDebug("%sushort %u", indent.toLocal8Bit().constData(), var.toUInt());
        (*out) += ("int." +QString::number(var.toInt()));
        break;
    case QMetaType::UInt:
        //            qDebug("%suint %u", indent.toLocal8Bit().constData(), var.toUInt());
        (*out) += ("int." +QString::number(var.toInt()));
        break;
    case QMetaType::Char:
        //            qDebug("%schar %d", indent.toLocal8Bit().constData(), var.toInt());
        (*out) += ("int." +QString::number(var.toInt()));
        break;
    case QMetaType::Short:
        //            qDebug("%sshort %d", indent.toLocal8Bit().constData(), var.toInt());
        (*out) += ("int." +QString::number(var.toInt()));
        break;
    case QMetaType::Int:
        //            qDebug("%sint %d", indent.toLocal8Bit().constData(), var.toInt());
        (*out) += ("int." +QString::number(var.toInt()));
        break;
    case QMetaType::QString:
        //            qDebug("%sstring %s", indent.toLocal8Bit().constData(), var.toString().toLocal8Bit().constData());
        (*out) += "string." + var.toString();
        break;
    case QMetaType::Bool:
        //            qDebug("%sbool %d", indent.toLocal8Bit().constData(), var.toBool());
        (*out) += ("bool." +QString::number(var.toInt()));
        break;
    case QMetaType::QUrl:
        //            qDebug("%surl %s", indent.toLocal8Bit().constData(), var.toUrl().toString().toLocal8Bit().constData());
        (*out) += "url." + var.toUrl().toString();
        break;
    case QVariant::UserType:
        if (var.userType() == qMetaTypeId<QBluetoothUuid>()) {
            QBluetoothUuid uuid = var.value<QBluetoothUuid>();
            switch (uuid.minimumSize()) {
            case 0:
                //                    qDebug("%suuid NULL", indent.toLocal8Bit().constData());
                break;
            case 2:
                //                    qDebug("%suuid %04x", indent.toLocal8Bit().constData(), uuid.toUInt16());
                (*out) += ("uuid16." + QString::number(uuid.toUInt16()));
                break;
            case 4:
                //                    qDebug("%suuid %08x", indent.toLocal8Bit().constData(), uuid.toUInt32());
                (*out) += ("uuid32." + QString::number(uuid.toUInt32()));
                break;
            case 16:
                //                    qDebug("%suuid %s", indent.toLocal8Bit().constData(), QByteArray(reinterpret_cast<const char *>(uuid.toUInt128().data), 16).toHex().constData());
                (*out) += "uuid128." + uuid.toString();
                break;
            default:
                //                    qDebug("%suuid ???", indent.toLocal8Bit().constData());
                ;
            }
        } else if (var.userType() == qMetaTypeId<QBluetoothServiceInfo::Sequence>()) {
            //                qDebug("%sSequence", indent.toLocal8Bit().constData());
            const QBluetoothServiceInfo::Sequence *sequence = static_cast<const QBluetoothServiceInfo::Sequence *>(var.data());
            (*out) += "(";
            int i=0;
            foreach (const QVariant &v, *sequence) {
                if (i>0) (*out) += ",";
                dumpAttributeVariant(v, out);
                i++;
            }
            (*out) += ")";
        } else if (var.userType() == qMetaTypeId<QBluetoothServiceInfo::Alternative>()) {

        }
        break;
    default:
        break;
    }
}
}

class BtScanner : public QObject
{
    Q_OBJECT
protected:
    BtScanner(bool mock, QObject *parent = 0) : QObject(parent)
    {
        qDebug() << "BtScanner created mock:"<<mock;
    }
public:
    BtScanner(QObject *parent = 0) :
        QObject(parent)
      ,m_timerStop()
      ,m_discoveryAgent(new QBluetoothServiceDiscoveryAgent(this))
      ,m_deviceDiscovery(new QBluetoothDeviceDiscoveryAgent(this))

    {
        connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
        connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
        connect(m_discoveryAgent, SIGNAL(error(QBluetoothServiceDiscoveryAgent::Error)), this, SLOT(onDiscoveryError(QBluetoothServiceDiscoveryAgent::Error)));


        connect(m_deviceDiscovery, &QBluetoothDeviceDiscoveryAgent::canceled
                , this, &BtScanner::deviceDiscoveryCanceled);
        connect(m_deviceDiscovery, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered
                , this, &BtScanner::onDeviceDiscovered);
        connect(m_deviceDiscovery, &QBluetoothDeviceDiscoveryAgent::deviceUpdated
                , this, &BtScanner::deviceUpdated);
        connect(m_deviceDiscovery, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error)
                , this, &BtScanner::onDeviceDiscoveryError);
        connect(m_deviceDiscovery, &QBluetoothDeviceDiscoveryAgent::finished
                , this, &BtScanner::deviceDiscoveryFinished);


        connect(&m_timerStop,SIGNAL(timeout()), this, SLOT(stopDiscovery()));
        m_timerStop.setSingleShot(true);
        m_timerStop.setInterval(90000); // arbitrarily long, perhaps should go

        connect(&m_timerStopDeviceDiscovery ,SIGNAL(timeout()), this, SLOT(stopDeviceDiscovery()));
        m_timerStopDeviceDiscovery.setSingleShot(true);
        m_timerStopDeviceDiscovery.setInterval(33000); // arbitrarily long, perhaps should go

        qDebug() << "BtScanner created";
    }

    ~BtScanner()
    {

    }

signals:
    void discoveryActive();
    void discoveryIsFinished(int);
    void discoveryError(const QString err);

    // device discovery
    void deviceDiscoveryActive();
    void deviceDiscoveryIsFinished(int count);
    void deviceDiscoveryError(const QString err);
    void deviceDiscovered(const QString deviceName);

public slots:
    // Clean the list, start the discovery

    void onDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error) {
        QString err;
        switch(error) {
        case (QBluetoothServiceDiscoveryAgent::NoError):
            err = "No error";
            break;
        case (QBluetoothServiceDiscoveryAgent::InputOutputError):
            err =  "Input/Output Error occurred during device discovery";
            break;
        case (QBluetoothServiceDiscoveryAgent::PoweredOffError):
            err =  "Powered Off Error occurred during device discovery";
            break;
        case (QBluetoothServiceDiscoveryAgent::InvalidBluetoothAdapterError):
            err =  "Invalid bluetooth adapter Error occurred during device discovery";
            break;
        case (QBluetoothServiceDiscoveryAgent::UnknownError):
            err =  "An unidentified error occurred during device discovery";
            break;
        }
        qDebug() << err;
        if (error != QBluetoothServiceDiscoveryAgent::NoError) {
            stopDiscovery();
            emit discoveryError(err);
        }
    }

    Q_INVOKABLE virtual void stopDiscovery()
    {
        if (m_discoveryAgent->isActive()) {
            qDebug() << "Scanning will be stopped!";
            m_discoveryAgent->stop();
            emit discoveryIsFinished(m_services.size());
        } else {
            qDebug() << "discovery already stopped!";
        }
    }

    // note: uuid filter and mask, in this case, must be provided in form of a 16 byte array, not in the
    // string form
    Q_INVOKABLE virtual void startDiscoveryByUuid(const QByteArray &addressFilter = {}, const QByteArray &filterMask = {})
    {
        // convert qbytearray into quint128
        quint128 filter = null128;
        quint128 mask = null128;
        if (addressFilter.size() == 16) {
            for (int i = 0; i < 16; ++i) {
                filter.data[i] = addressFilter.at(i);
            }
        }
        if (filterMask.size() == 16) {
            for (int i = 0; i < 16; ++i) {
                mask.data[i] = filterMask.at(i);
            }
        }
        startDiscoveryUUID(filter, mask);
    }

    Q_INVOKABLE virtual void startDiscoveryByName(const QString &nameFilter = {})
    {
        m_nameFilter = nameFilter;
        startDiscovery();
    }

    virtual void startDiscoveryUUID(quint128 addressFilter = null128, quint128 filterMask = null128)
    {
        m_filterTemplate = addressFilter & filterMask;
        m_filterMask = filterMask;
        startDiscovery();
    }
    virtual void startDiscovery()
    {
        if (m_discoveryAgent->isActive()) {
            qDebug() << "Scanning is already ongoing!";
            return;
        }

        m_services.clear();
        m_servicesNames.clear();

        m_discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
        qDebug() << "Scanning started!";
        m_timerStop.start();
        emit discoveryActive();
    }

    // Add a service to a list
    virtual void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
    {
#if 0
        qDebug() << "Discovered service!!!";

        qDebug() << "Discovered service on"
                 << serviceInfo.device().name() << serviceInfo.device().address().toString();
        qDebug() << "\tService name:" << serviceInfo.serviceName();
        qDebug() << "\tDescription:"qDebug() << "Discovered service on"
                 << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
        qDebug() << "\tProvider:"
                 << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
        qDebug() << "\tL2CAP protocol service multiplexer:"
                 << serviceInfo.protocolServiceMultiplexer();
        qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();
#endif

        m_services.append(serviceInfo);
    }

    Q_INVOKABLE virtual int serviceCount() const {
        return m_services.size();
    }



    Q_INVOKABLE virtual QBluetoothServiceInfo getServiceInfo(unsigned int idx) {
        if (idx >= m_services.size())
            return QBluetoothServiceInfo();
        return m_services[idx];
    }
    Q_INVOKABLE virtual QString getServiceName(unsigned int idx)
    {
        if (idx >= m_services.size()) return QString("");
        return m_servicesNames[idx];
    }

    Q_INVOKABLE virtual void startReceiving(unsigned int idx, BtReceiver *btreceiver)
    {
        if (idx >= m_services.size()) {
            // qDebug() << "idx >= m_services.size()";
            return;
        }
        if (btreceiver) {
            btreceiver->stopReceiver();
            btreceiver->startReceiver(m_services[idx]);
        }
    }

#if 0
    Q_INVOKABLE void startReceiving(QBluetoothServiceInfo bsi)
    {
        if (! bsi.isValid()) {
            qDebug() << "QBluetoothServiceInfo not valid";
            return;
        }
        if (m_btreceiver) {
            m_btreceiver->stopReceiver();
            m_btreceiver->startReceiver(bsi);
        }
    }
#endif

    Q_INVOKABLE virtual void startDeviceDiscoveryByName(const QString &nameFilter = {})
    {
        clearDeviceFilters();
        m_deviceNameFilter = nameFilter;
        startDeviceDiscovery();
    }

    Q_INVOKABLE virtual void stopDeviceDiscovery()
    {
        if (m_deviceDiscovery->isActive()) {
            qDebug() << "stopDeviceDiscovery: Device scanning will be stopped!";
            m_deviceDiscovery->stop();
            deviceDiscoveryFinished();
        } else {
            qDebug() << "Device discovery already stopped!";
            deviceDiscoveryFinished();
        }
    }

    Q_INVOKABLE virtual int devicesCount() const {
        return m_devices.size();
    }

    Q_INVOKABLE virtual QBluetoothDeviceInfo getDeviceInfo(unsigned int idx) {
        if (idx >= m_devices.size())
            return QBluetoothDeviceInfo();
        return m_devices[idx];
    }
    Q_INVOKABLE virtual QString getDeviceName(unsigned int idx)
    {
        if (idx >= m_devices.size()) return QString("");
        return m_devices[idx].name();
    }

    Q_INVOKABLE virtual void startReceivingFromDevice(unsigned int deviceIdx, const QString serviceUUID, BtReceiver &btreceiver)
    {
        if (deviceIdx >= m_devices.size()) {
            qDebug() << "idx >= m_devices.size()";
            return;
        }
        {
            btreceiver.stopReceiver();

            QBluetoothServiceInfo sinfo;
            sinfo.setDevice(m_devices[deviceIdx]);
            sinfo.setServiceUuid(QBluetoothUuid(serviceUUID));
            if (!sinfo.isValid()) {
                qDebug() << "Error: trying to connect to an invalid service info!";
                return;
            }
            btreceiver.startReceiver(sinfo);
        }
    }

    Q_INVOKABLE virtual QBluetoothServiceInfo infoFromDevice(unsigned int deviceIdx, const QString serviceUUID)
    {
        if (deviceIdx >= m_devices.size()) {
            qDebug() << "idx >= m_devices.size()";
            return {};
        }
        {
            QBluetoothServiceInfo sinfo;
            sinfo.setDevice(m_devices[deviceIdx]);
            sinfo.setServiceUuid(QBluetoothUuid(serviceUUID));
            if (!sinfo.isValid()) {
                qDebug() << "Error: trying to connect to an invalid service info!";
                return {};
            }
            return sinfo;
        }
    }

    static QString serializeBluetoothServiceInfo(QBluetoothServiceInfo info)
    {
        static const QString bsisep = "##BtServiceInfoSep##";
        static const QString bsiattrsep = "##BtServiceInfoAttrSep##";

        QString res = "";

        //Serialize QBluetoothDeviceInfo

        res += QString(info.device().name());
        res += bsisep;
        res += QString::number(info.device().address().toUInt64());
        res += bsisep;
        res += QString::number(info.device().majorDeviceClass());
        res += bsisep;
        res += QString::number( info.device().minorDeviceClass() );
        res += bsisep;
        res += QString::number( info.device().serviceClasses() );
        res += bsisep;

        //Serialize ServiceInfo Attrs
        int i =0;
        foreach (quint16 attr, info.attributes ()) {
            if (i)
                res += bsiattrsep ;
            res += ( QString::number(attr) + ".");
            dumpAttributeVariant(info.attribute (attr), &res);
            i++;
        }

        return res;
    }

    static QBluetoothServiceInfo deserializeBluetoothServiceInfo(QString serializedinfo)
    {
        static const QString bsisep = "##BtServiceInfoSep##";
        static const QString bsiattrsep = "##BtServiceInfoAttrSep##";
        /*  Bits	Size	Description
            0 - 1	2	Unused, set to 0.
            2 - 7	6	Minor device class.
            8 - 12	5	Major device class.
            13 - 23	11	Service class.
        */

        QStringList bsiparts = serializedinfo.split(bsisep);
        // 0 dev Name
        QString devname = bsiparts[0];
        // 1 dev address
        QBluetoothAddress devaddress = QBluetoothAddress(bsiparts[1].toULongLong());
        // 2 dev major
        quint32 majorDevClass = bsiparts[2].toInt();
        // 3 dev minor
        quint32 minorDevClass = bsiparts[3].toInt();
        // 4 dev serviceClasses
        // We could skip this for now, its not used anyway

        minorDevClass = minorDevClass << 2;
        majorDevClass = majorDevClass << 8;
//        serviceclass = serviceclass << 13;

        quint32 classOfDevice = 0;
        classOfDevice |= minorDevClass;
        classOfDevice |= majorDevClass;
//        classOfDevice |= serviceclass;

        QBluetoothDeviceInfo dev = QBluetoothDeviceInfo( devaddress
                                 ,devname
                                 ,classOfDevice);

        QBluetoothServiceInfo bsi;
        bsi.setDevice(dev);

        // 5 service attribs
        QStringList attrs = bsiparts[5].split(bsiattrsep);
        for (int i=0; i< attrs.size(); i++) {
            QString id = attrs[i].left(attrs[i].indexOf('.'));
            QString attr = attrs[i].right(attrs[i].size() - attrs[i].indexOf('.') -1 );
            int attrid = id.toInt();
            QVariant attrv = parseAttribute(attr);
            bsi.setAttribute(attrid,attrv);
        }
        return bsi;
    };

private slots:
    void discoveryFinished()
    {
        qDebug() << "Discovery finished! srvcs "<<m_services.size();

        debugServices();
        // filter services
        QList<QBluetoothServiceInfo> filtered;
        m_servicesNames.clear();
        for (const auto &s: m_services)
        {
            const quint128 uuid = s.serviceUuid().toUInt128() & m_filterMask;
            if (m_filterTemplate != null128 && uuid != m_filterTemplate) {
                continue;
            }
#ifdef Q_OS_ANDROID
            // workaround: match also the inverse, as android up to some version invert custom UUIDs
            // ToDo: further filter by android version? the bug must have been fixed at some point

#endif
            if (!m_nameFilter.isEmpty() && s.serviceName() != m_nameFilter)
                continue;

            filtered.append(s);
            QString device;
            if (s.device().name().isEmpty()) {
                device = s.device().address().toString();
            } else {
                device = s.device().name();
            }

            QString service;
            if (!s.serviceName().isEmpty())
                service = s.serviceName();

            if (!s.serviceDescription().isEmpty())
                service += QLatin1String(" (") + s.serviceDescription() + QLatin1String(")");

              m_servicesNames.append(device + "::" + service);
        }
        m_services = filtered;
        clearFilters();
        emit discoveryIsFinished(m_services.size());
    }


    void debugServices() {
        // print on console details of services
        for (int i = 0; i < m_services.size(); ++i) {
            auto info = m_services.at(i);
            auto device = info.device();

            qDebug() << "== Service " << i << " ==";
            qDebug() << "dev name" << device.name() << " -- " << device.address().toString();
            qDebug() << "dev UUID " << device.deviceUuid();
            qDebug() << "rssi " << device.rssi();
            qDebug() << "classes " << device.serviceClasses();
            qDebug() << "major " << device.majorDeviceClass();
            qDebug() << "minor " << device.minorDeviceClass();
            qDebug() << "dev service UUIDs";
            for (auto u: device.serviceUuids())
                qDebug() << " "<< u;
            qDebug() << "serv name " << info.serviceName();
            qDebug() << "serv desc " << info.serviceDescription();
            qDebug() << "serv prov " << info.serviceProvider();
            qDebug() << "serv uuid " << info.serviceUuid();
            qDebug() << "correct" << (info.serviceUuid() ==
                QBluetoothUuid(QLatin1String("e8e10f95-1a70-4b27-9ccf-02010264e9c3")));


            QString attrs;
            int ii =0;
            foreach (quint16 attr, info.attributes ()) {
                if (ii) attrs += "##" ;
                attrs += ( QString::number(attr) + ".");
                dumpAttributeVariant(info.attribute (attr), &attrs);
                ii++;
            }
            qDebug() << "service attrs" << attrs;
        }
    }

    void debugDevices() {
        // print on console details of services
        for (int i = 0; i < m_devices.size(); ++i) {
            auto device = m_devices.at(i);

            qDebug() << "== Device " << i << " ==";
            qDebug() << "dev name" << device.name() << " -- " << device.address().toString();
            qDebug() << "dev UUID " << device.deviceUuid();
            qDebug() << "rssi " << device.rssi();
            qDebug() << "classes " << device.serviceClasses();
            qDebug() << "major " << device.majorDeviceClass();
            qDebug() << "minor " << device.minorDeviceClass();
            qDebug() << "dev service UUIDs";
            for (auto u: device.serviceUuids())
                qDebug() << " "<< u;
        }
    }


    /*  Device discovery */
    void deviceDiscoveryCanceled()
    {
        qDebug() << Q_FUNC_INFO;
        m_devices.clear();
        m_timerStopDeviceDiscovery.stop();
    }

    void onDeviceDiscovered(const QBluetoothDeviceInfo &info)
    {
        if (findDevice(info.address()) >= 0) {
            qDebug() << "Device already added: skipping";
            return;
        }
        m_devices.append(info);
        emit deviceDiscovered(info.name() != "" ? info.name()
                                                : (info.deviceUuid().toString() == "{00000000-0000-0000-0000-000000000000}"
                                                   ? QStringLiteral("<unknown device %1>").arg(m_devices.count())
                                                   : info.deviceUuid().toString()));
    }

    void deviceUpdated(const QBluetoothDeviceInfo &/*info*/, QBluetoothDeviceInfo::Fields /*updatedFields*/)
    {
        // ignore
    }

    void onDeviceDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error error)
    {
        m_timerStopDeviceDiscovery.stop();
        QString err;
        switch(error) {
        case (QBluetoothDeviceDiscoveryAgent::NoError):
            err = "No error";
            break;
        case (QBluetoothDeviceDiscoveryAgent::InputOutputError):
            err =  "Input/Output Error occurred during device discovery";
            break;
        case (QBluetoothDeviceDiscoveryAgent::PoweredOffError):
            err =  "Powered Off Error occurred during device discovery";
            break;
        case (QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError):
            err =  "Invalid bluetooth adapter Error occurred during device discovery";
            break;
        case (QBluetoothDeviceDiscoveryAgent::UnsupportedPlatformError):
            err =  "UnsupportedPlatform Error occurred during device discovery";
            break;
        case (QBluetoothDeviceDiscoveryAgent::UnsupportedDiscoveryMethod):
            err =  "UnsupportedDiscoveryMethod Error occurred during device discovery";
            break;
        case (QBluetoothDeviceDiscoveryAgent::UnknownError):
            err =  "An unidentified error occurred during device discovery";
            break;
        }
        qDebug() << err;
        if (error != QBluetoothDeviceDiscoveryAgent::NoError) {
            stopDeviceDiscovery();
            emit deviceDiscoveryError(err);
        }
    }

    void deviceDiscoveryFinished()
    {
        qDebug() << "Device discovery finished! devices "<<m_devices.size();
        m_timerStopDeviceDiscovery.stop();
        debugDevices();
        // filter services
        QList<QBluetoothDeviceInfo> filtered;
        if (!m_deviceNameFilter.isEmpty()) {
            qDebug() << "Applying filter: "<<m_deviceNameFilter;
        }
        for (const auto &s: m_devices)
        {
            const quint128 uuid = s.deviceUuid().toUInt128() & m_deviceFilterMask;
            if (m_deviceFilterTemplate != null128 && uuid != m_deviceFilterTemplate) {
                qDebug() << "Skipping "<<s.name()<< " not matching uuid template";
                continue;
            } else {
                qDebug() << "Matched "<<s.name() << " : " << s.deviceUuid();
            }
#ifdef Q_OS_ANDROID
            // workaround: match also the inverse, as android up to some version invert custom UUIDs
            // ToDo: further filter by android version? the bug must have been fixed at some point

#endif
            if (!m_deviceNameFilter.isEmpty() && s.name().indexOf(m_deviceNameFilter) < 0) {
                qDebug() << "Skipping "<<s.name()<< " not matching name template";
                continue;
            } else {
                qDebug() << "Matched "<<s.name() << " : " << s.name().indexOf(m_deviceNameFilter);
            }

            filtered.append(s);

        }
        m_devices = filtered;
        clearFilters();
        emit deviceDiscoveryIsFinished(m_devices.size());
    }

protected:
    void startDeviceDiscovery()
    {
        if (m_deviceDiscovery->isActive()) {
            qDebug() << "Device scanning is already ongoing!";
            return;
        }

        m_devices.clear();
        m_deviceDiscovery->stop();

        m_deviceDiscovery->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
        qDebug() << "Scanning started!";
        //m_timerStopDeviceDiscovery.start();
        emit deviceDiscoveryActive();
    }

    void clearFilters()
    {
        m_filterTemplate = m_filterMask = null128;
        m_nameFilter.clear();
    }

    void clearDeviceFilters()
    {
        m_deviceFilterTemplate = m_deviceFilterMask = null128;
        m_deviceNameFilter.clear();
    }

    int findDevice(const QBluetoothAddress &address) {
        for (int i = 0;  i< m_devices.size(); ++i)
            if (m_devices.at(i).address() == address)
                return i;
        return -1;
    }

    QTimer m_timerStop;
    QBluetoothServiceInfo serviceInfo;
    QBluetoothServiceDiscoveryAgent *m_discoveryAgent = nullptr;
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscovery = nullptr;

    QList<QBluetoothServiceInfo> m_services;
    QList<QString> m_servicesNames;
    quint128 m_filterTemplate = null128;
    quint128 m_filterMask = null128;
    QString m_nameFilter;

    QTimer m_timerStopDeviceDiscovery;
    QList<QBluetoothDeviceInfo> m_devices;
    quint128 m_deviceFilterTemplate = null128;
    quint128 m_deviceFilterMask = null128;
    QString m_deviceNameFilter;
};


class BtScannerMock : public BtScanner {
    Q_OBJECT
public:
    BtScannerMock(QObject *parent = nullptr) : BtScanner(true, parent)
    {

    }

    virtual void startDeviceDiscoveryByName(const QString &nameFilter = {})
    {
        m_deviceNameFilter = nameFilter;


        QBluetoothDeviceInfo dev = QBluetoothDeviceInfo( QBluetoothAddress{"123"}
                                 ,"qlappie"
                                 ,1);
        dev.setDeviceUuid(QBluetoothUuid(QLatin1String("a5e21111-0101-cfcf-274b-0000950fe1e8")));

        m_devices.append(dev);

        QBluetoothServiceInfo service;
        service.setDevice(dev);
        service.setServiceUuid(QBluetoothUuid(QLatin1String("c3e96402-0102-cf9c-274b-701a950fe1e8")));
        m_services.append(service);

        emit deviceDiscoveryActive();
        emit deviceDiscoveryIsFinished(1);
    }

    virtual QString getDeviceName(unsigned int idx)
    {
        if (idx >= m_devices.size()) return QString("");
        return m_devices[idx].name();
    }

    virtual QBluetoothDeviceInfo getDeviceInfo(unsigned int idx) {
        if (idx >= m_devices.size())
            return QBluetoothDeviceInfo();
        return m_devices[idx];
    }

    virtual int devicesCount() const {
        return m_devices.size();
    }
};

#endif // BTSCANNER_H
