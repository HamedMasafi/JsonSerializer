#include "myobject.h"

MyObject::MyObject(QObject *parent)
    : QObject{parent}
{}

int MyObject::id() const
{
    return m_id;
}

void MyObject::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString MyObject::name() const
{
    return m_name;
}

void MyObject::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QColor MyObject::backgroundColor() const
{
    return m_backgroundColor;
}

void MyObject::setBackgroundColor(const QColor &newBackgroundColor)
{
    if (m_backgroundColor == newBackgroundColor)
        return;
    m_backgroundColor = newBackgroundColor;
    emit backgroundColorChanged();
}

bool MyObject::operator==(const MyObject &other) const
{
    return m_id == other.m_id && m_name == other.m_name
           && m_backgroundColor == other.m_backgroundColor
           && m_gadget == other.m_gadget;
}

MyGadget *MyObject::gadget() const
{
    return m_gadget;
}

void MyObject::setGadget(MyGadget *newGadget)
{
    if (m_gadget == newGadget)
        return;
    m_gadget = newGadget;
    emit gadgetChanged();
}
