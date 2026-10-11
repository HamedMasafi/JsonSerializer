#pragma once

#include <QObject>
#include <QColor>
#include "mygadget.h"

class MyObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY
                   backgroundColorChanged FINAL)
    Q_PROPERTY(MyGadget* gadget READ gadget WRITE setGadget NOTIFY gadgetChanged FINAL)

public:
    Q_INVOKABLE MyObject(QObject *parent = nullptr);

    int id() const;
    void setId(int newId);

    QString name() const;
    void setName(const QString &newName);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    bool operator==(const MyObject &other) const;
    MyGadget *gadget() const;
    void setGadget(MyGadget *newGadget);

signals:
    void idChanged();

    void nameChanged();

    void backgroundColorChanged();

    void gadgetChanged();

private:
    int m_id;
    QString m_name;
    QColor m_backgroundColor;
    MyGadget *m_gadget = nullptr;
};
