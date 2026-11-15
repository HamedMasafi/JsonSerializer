#pragma once

#include "animalobject.h"

class TigerObject : public AnimalObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit TigerObject(QObject *parent = nullptr);
};
