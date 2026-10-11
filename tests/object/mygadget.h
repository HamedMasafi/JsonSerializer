#pragma once

#include <QObject>

class MyGadget
{
    Q_GADGET
    Q_PROPERTY(int id READ id WRITE setId)

public:
    MyGadget();
    int id() const;
    void setId(int newId);

private:
    int m_id;
};
