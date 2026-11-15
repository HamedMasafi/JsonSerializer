#pragma once

#include <QJsonObject>

#include "options.h"

namespace JsonSerializer::Impl {

QJsonObject objectToJson(QObject *obj);

bool jsonToObject(const QJsonObject &json, QObject *obj);
QObject *createQObject(const QString &className);

QJsonObject gadgetToJson(const void *obj, const QMetaObject &metaObject);
bool jsonToGadget(const QJsonObject &json, void *obj, const QMetaObject &metaObject);
QVariant createGadget(const QString &className);

template<class T>
T *jsonToObject(const QJsonObject &obj)
{
    if (obj.contains(options.className)) {
        QObject *instance;
        auto className = obj[options.className].toString();
        if (className.endsWith("*"))
            instance = createQObject(className);
        else
            instance = createQObject(className + "*");

        if (!instance)
            return nullptr;

        if (!jsonToObject(obj, instance))
            return nullptr;
        return qobject_cast<T *>(instance);
    } else {
        auto instance = new T;
        if (!jsonToObject(obj, instance))
            return nullptr;
        return instance;
    }
}

} // namespace JsonSerializer::Impl
