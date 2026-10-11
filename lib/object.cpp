#include "object.h"

#include <QMetaProperty>
#include "variant.h"

namespace JsonSerializer::Impl {

QJsonObject objectToJson(QObject *obj)
{
    QJsonObject jsonObject;
    auto me = obj->metaObject();

    for (int i = 0; i < me->propertyCount(); ++i) {
        auto p = me->property(i);
        if (p.isEnumType())
            jsonObject[QString{p.name()}] = variantToJson(p.read(obj).toInt());
        else
            jsonObject[QString{p.name()}] = variantToJson(p.read(obj));
    }

    auto dynamicNames = obj->dynamicPropertyNames();
    if (!dynamicNames.isEmpty()) {
        QJsonObject dynamicPropertiesObject;
        for (const auto &name : std::as_const(dynamicNames)) {
            dynamicPropertiesObject[name] = variantToJson(obj->property(name));
        }
        jsonObject["__dynamicProperties"] = dynamicPropertiesObject;
    }

    return jsonObject;
}

bool jsonToObject(const QJsonObject &json, QObject *obj)
{
    auto me = obj->metaObject();

    for (int i = 0; i < me->propertyCount(); ++i) {
        auto p = me->property(i);
        p.write(obj, jsonToVariant(json[p.name()], static_cast<QMetaType::Type>(p.typeId())));
    }

    return true;
}


QJsonObject gadgetToJson(const void *obj, const QMetaObject &metaObject)
{
    QJsonObject jsonObject;

    for (int i = 0; i < metaObject.propertyCount(); ++i) {
        auto p = metaObject.property(i);
        if (p.isEnumType())
            jsonObject[QString{p.name()}] = variantToJson(p.readOnGadget(obj).toInt());
        else
            jsonObject[QString{p.name()}] = variantToJson(p.readOnGadget(obj));
    }

    jsonObject["__className"] = metaObject.className();
    return jsonObject;
}

bool jsonToGadget(const QJsonObject &json, void *obj, const QMetaObject &metaObject)
{
    for (int i = 0; i < metaObject.propertyCount(); ++i) {
        auto p = metaObject.property(i);
        if (json.contains(p.name()))
            p.writeOnGadget(obj,
                            jsonToVariant(json[p.name()], static_cast<QMetaType::Type>(p.typeId())));
    }
    return true;
}

QObject *createQObject(const QString &className)
{
    const auto *metaObject = QMetaType::fromName(className.toLatin1()).metaObject();

    if (!metaObject || !metaObject->inherits(&QObject::staticMetaObject))
        return nullptr;

    return metaObject->newInstance();
}

QVariant createGadget(const QString &className)
{
    const auto metaType = QMetaType::fromName(className.toLatin1());

    if (!metaType.isValid() || !metaType.metaObject())
        return {};

    return metaType.create() ? QVariant(metaType, metaType.create()) : QVariant{};
}
}
