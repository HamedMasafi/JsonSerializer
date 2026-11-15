#pragma once

#include <QObject>

class AnimalObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

public:
    Q_INVOKABLE explicit AnimalObject(QObject *parent = nullptr);
    virtual ~AnimalObject() = default;

    QString name() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private:
    QString m_name;
};
