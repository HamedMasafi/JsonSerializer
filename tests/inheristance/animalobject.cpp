#include "animalobject.h"

AnimalObject::AnimalObject(QObject *parent)
    : QObject{parent}
{}

QString AnimalObject::name() const
{
    return m_name;
}

void AnimalObject::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}
