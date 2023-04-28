#if 1
#ifndef BTSERVICEITEM_H
#define BTSERVICEITEM_H

#include "listmodel.h"
//#include <QVariant>
//#include <QHash>

class BtServiceItem : public ListItem
{
  Q_OBJECT

public:
  enum Roles {
    NameRole = Qt::UserRole+1,
    IndexRole
  };


public:
  BtServiceItem(QObject *parent = 0): ListItem(parent) {}
  explicit BtServiceItem(const QString &name, const quint32 &index, QObject *parent = 0):
      ListItem(parent), m_name(name), m_index(index)
  {

  }
  QVariant data(int role) const
  {
    switch(role) {
        case NameRole:
          return name();
        case IndexRole:
          return index();
        default:
          return QVariant();
    }
  };
  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[IndexRole] = "index";
    return names;
  };
  void setIndex(quint32 index);
  inline QString id() const { return m_name; }
  inline QString name() const { return m_name; }
  inline quint32 index() const { return m_index; }

private:
  QString m_name;
  quint32 m_index;
};

#endif // BTSERVICEITEM_H
#endif
