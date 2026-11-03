#include "enum.h"
#include "options.h"

namespace JsonSerializer::Impl {

QJsonValue enumVariantToJson(const QVariant &v)
{
    auto mt = QMetaType(v.userType());

    auto v2 = v;
    v2.convert(mt.underlyingType());
    return QJsonValue::fromVariant(v2);
}

QJsonValue enumTypeToJson(const QMetaEnum &en, const QVariant &v)
{
    if (options.exportEnumAsString) {
        if (en.isValid()) {
            if (en.isFlag())
                return QJsonValue{QString{en.valueToKeys(v.value<quint64>())}};
            return QJsonValue{en.valueToKey(v.value<quint64>())};
        }
        return QJsonValue::fromVariant(v);
    } else {
        return QJsonValue::fromVariant(v.toInt());
    }
}

QVariant jsonToEnumType(const QMetaEnum &en, const QJsonValue &v)
{
    if (options.exportEnumAsString && en.isValid())
        return en.keysToValue(v.toString().toUtf8());
    return v.toInt();
}

} // namespace JsonSerializer::Impl