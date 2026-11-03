#pragma once

#include <QJsonValue>
#include <QMetaEnum>
#include <QMetaType>

#include "options.h"
#include "traits.h"

namespace JsonSerializer::Impl {

QJsonValue enumTypeToJson(const QMetaEnum &en, const QVariant &v);
QVariant jsonToEnumType(const QMetaEnum &en, const QJsonValue &v);

template<typename T>
QJsonValue enumToJson(const T &en)
{
    if constexpr (IsQEnum<T>::value) {
        auto me = QMetaEnum::fromType<T>();
        return enumTypeToJson(me, en);
        //   return QJsonValue{me.valueToKey(static_cast<quint64>(en))};
    } else {
        return QJsonValue{int(en)};
    }
}

template<typename T>
T jsonToEnum(const QJsonValue &v)
{
    if constexpr (IsQEnum<T>::value) {
        auto me = QMetaEnum::fromType<T>();
        return jsonToEnumType(me, v).template value<T>();
        // return static_cast<T>(me.keyToValue(v.toString().toUtf8()));
        return static_cast<T>(v.toInt());
    } else {
        return T(v.toDouble());
    }
}

QJsonValue enumVariantToJson(const QVariant &v);

} // namespace JsonSerializer::Impl
