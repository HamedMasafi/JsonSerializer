#include "mygadget.h"

MyGadget::MyGadget() {}

int MyGadget::id() const
{
    return m_id;
}

void MyGadget::setId(int newId)
{
    m_id = newId;
}
