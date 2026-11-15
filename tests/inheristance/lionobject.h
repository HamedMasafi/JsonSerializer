#pragma once

#include "animalobject.h"

class LionObject : public AnimalObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit LionObject(QObject *parent = nullptr);
};
