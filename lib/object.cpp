#include "object.h"

#include <QMetaProperty>
#include "enum.h"
#include "options.h"
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

    jsonObject[options.className] = obj->metaObject()->className();

    return jsonObject;
}

bool jsonToObject(const QJsonObject &json, QObject *obj)
{
    auto me = obj->metaObject();

    for (int i = 0; i < me->propertyCount(); ++i) {
        auto p = me->property(i);
        auto v = jsonToVariant(json[p.name()], static_cast<QMetaType::Type>(p.typeId()));
        auto ok = p.write(obj, v);

        if (!ok)
            qWarning() << "Unable to write" << p.name() << "value=" << v;
    }

    return true;
}

QJsonObject gadgetToJson(const void *obj, const QMetaObject &metaObject)
{
    QJsonObject jsonObject;

    for (int i = 0; i < metaObject.propertyCount(); ++i) {
        auto p = metaObject.property(i);
        if (p.isEnumType())
            jsonObject[QString{p.name()}] = enumTypeToJson(p.enumerator(), p.readOnGadget(obj));
        else
            jsonObject[QString{p.name()}] = variantToJson(p.readOnGadget(obj));
    }

    QString className;
    for (auto i = 0; i < metaObject.classInfoCount(); ++i) {
        if (!strcmp(metaObject.classInfo(i).name(), "jsonserializer_type")) {
            className = metaObject.classInfo(i).value();
        }
    }
    if (className.isEmpty())
        className = metaObject.className();
    jsonObject[options.className] = className;
    return jsonObject;
}

bool jsonToGadget(const QJsonObject &json, void *obj, const QMetaObject &metaObject)
{
    for (int i = 0; i < metaObject.propertyCount(); ++i) {
        auto p = metaObject.property(i);
        if (json.contains(p.name()))
            if (p.isEnumType())
                p.writeOnGadget(obj, jsonToEnumType(p.enumerator(), json[p.name()]));
            else
                p.writeOnGadget(obj,
                                jsonToVariant(json[p.name()],
                                              static_cast<QMetaType::Type>(p.typeId())));
    }
    return true;
}

QObject *createQObject(const QString &className)
{
    const auto *metaObject = QMetaType::fromName(className.toLatin1()).metaObject();

    if (!metaObject || !metaObject->inherits(&QObject::staticMetaObject))
        return nullptr;
    auto r = metaObject->newInstance();
    if (!r)
        qWarning().noquote().nospace()
            << "Unable to create " << className
            << " class. Check that the classes default constructor is Q_INVOKABLE.";
    return r;
}

QVariant createGadget(const QString &className)
{
    const auto metaType = QMetaType::fromName(className.toLatin1());

    if (!metaType.isValid() || !metaType.metaObject())
        return {};

    return metaType.create() ? QVariant(metaType, metaType.create()) : QVariant{};
}


} // namespace JsonSerializer::Impl
