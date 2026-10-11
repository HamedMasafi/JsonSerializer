#pragma once

#include <QBuffer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMetaType>
#include <QString>
#include <QVariant>

#include "traits.h"

#include <type_traits>

// #include "jsonserialize_def.h"
namespace JsonSerializer {

template<typename T>
QJsonValue toJson(const T &v);

template<typename T>
T fromJson(const QJsonValue &v);

} // namespace JsonSerializer


#include "object.h"
#include "qtcontainers.h"
#include "qttypes.h"
#include "variant.h"

namespace JsonSerializer {

namespace Impl {}

// ------------------------------------------------------------
// QMap<QString, T>
// ------------------------------------------------------------

template<typename T>
struct IsQStringMap : std::false_type
{};

template<typename T>
struct IsQStringMap<QMap<QString, T>> : std::true_type
{
    using ValueType = T;
};

class Serializable
{
public:
    virtual void saveState(QJsonObject &) = 0;
    virtual void restoreState(const QJsonObject &) = 0;
};


template<typename T>
QJsonValue toJson(const T &v)
{
    using namespace Impl;
    using Raw = std::remove_cv_t<std::remove_reference_t<T>>;
    using U = std::remove_pointer_t<Raw>; // حالا U همیشه خود کلاس است

    if constexpr (Impl::IsQtTypeType<U>::value)
        return Impl::basicTypeToJson(v);
    else if constexpr (std::is_base_of_v<QObject, U>)
        return Impl::objectToJson(v);
    else if constexpr (Impl::IsQGadget<U>)
        return Impl::gadgetToJson(v, U::staticMetaObject);
    else if constexpr (std::is_base_of_v<Serializable, U>) {
        QJsonObject o;
        v->saveState(o);
        return o;
    // } else if constexpr (std::is_same_v<U, QVariantMap>) {
    //     QJsonObject obj;
    //     for (auto it = v.begin(); it != v.end(); ++it)
    //         obj[it.key()] = Impl::variantToJson(it.value());
    //     return obj;
    // } else if constexpr (IsQStringMap<U>::value) {
    //     QJsonObject result;

    //     for (auto it = v.cbegin(); it != v.cend(); ++it)
    //         result.insert(it.key(), toJson(it.value()));

    //     return result;
    } else if constexpr (std::is_same_v<QVariant, U>) {
        return packVariantToJson(v);
    }

    // clang-format off
    else if constexpr (IsQSharedPointer<U>::value)   return toJson(v.data());
    else if constexpr (IsStdPointer<U>::value)       return toJson(v.data());

    else if constexpr (Impl::IsQPair<U>::value)      return pairToJson<U>(v);
    else if constexpr (Impl::IsQList<U>::value)      return listToJson(v);
    else if constexpr (Impl::IsQVector<U>::value)    return vectorToJson(v);
    else if constexpr (Impl::IsQMap<U>::value)       return mapToJson<U>(v);
    else if constexpr (Impl::IsQMultiMap<U>::value)  return multiMapToJson(v);
    else if constexpr (Impl::IsQStack<U>::value)     return stackToJson(v);
    else if constexpr (Impl::IsQQueue<U>::value)     return queueToJson(v);
    else if constexpr (Impl::IsQSet<U>::value)       return setToJson(v);
    else if constexpr (Impl::IsQHash<U>::value)      return hashToJson(v);

    else if constexpr (Impl::IsStdVector<U>::value)             return stdVectorToJson(v);
    else if constexpr (Impl::IsStdList<U>::value)               return stdListToJson(v);
    else if constexpr (Impl::IsStdDeque<U>::value)              return stdDequeToJson(v);
    else if constexpr (Impl::IsStdSet<U>::value)                return stdSetToJson(v);
    else if constexpr (Impl::IsStdMultiSet<U>::value)           return stdMultiSetToJson(v);
    else if constexpr (Impl::IsStdUnorderedSet<U>::value)       return stdUnorderedSetToJson(v);
    else if constexpr (Impl::IsStdUnorderedMultiSet<U>::value)  return stdUnorderedMultiSetToJson(v);
    else if constexpr (Impl::IsStdMap<U>::value)                return stdMapToJson(v);
    else if constexpr (Impl::IsStdMultiMap<U>::value)           return stdMultiMapToJson(v);

    // clang-format on

    return QJsonValue::fromVariant(QVariant::fromValue(v));
}

template<typename T>
T fromJson(const QJsonValue &v)
{
    using namespace Impl;
    using Raw = std::remove_cv_t<std::remove_reference_t<T>>;
    using U = std::remove_pointer_t<Raw>; // حالا U همیشه خود کلاس است

    if constexpr (Impl::IsQtTypeType<U>::value) {
        return Impl::jsonToBasicType<T>(v);
    } else if constexpr (std::is_base_of_v<QObject, U>) {
        auto o = new U;
        if (!Impl::jsonToObject(v.toObject(), o))
            return nullptr;
        return o;
    } else if constexpr (Impl::IsQGadget<U>) {
        auto o = new U;
        Impl::jsonToGadget(v.toObject(), o, U::staticMetaObject);
        return o;
    } else if constexpr (std::is_base_of_v<Serializable, U>) {
        auto o = new U;
        v->restoreState(v.toObject());
        return o;
    } else if constexpr (std::is_same_v<QVariant, U>){
        return unpackJsonToVariant(v.toObject());
    } else if constexpr (Impl::IsQSharedPointer<U>::value) {
        using DataType = typename Impl::IsQSharedPointer<U>::DataType;
        return QSharedPointer<DataType>{fromJson<DataType *>(v)};
    } else if constexpr (Impl::IsStdPointer<U>::value) {
        using DataType = typename Impl::IsStdPointer<U>::DataType;
        return std::shared_ptr<DataType>{fromJson<DataType *>(v)};
    }

    // clang-format off
    else if constexpr (IsQList<U>::value)      return jsonToList<typename IsQList<T>::ItemType>(v);
    else if constexpr (IsQVector<U>::value)    return jsonToVector<typename IsQVector<U>::ItemType>(v);
    else if constexpr (IsQSet<U>::value)       return jsonToSet<typename IsQSet<U>::ItemType>(v);
    else if constexpr (IsQQueue<U>::value)     return jsonToQueue<typename IsQQueue<U>::ItemType>(v);
    else if constexpr (IsQPair<U>::value)      return jsonToPair<T>(v);
    else if constexpr (IsQMap<U>::value)       return jsonToMap<T>(v);
    else if constexpr (IsQMultiMap<U>::value)  return jsonToMultiMap<T>(v);
    else if constexpr (IsQStack<U>::value)     return jsonToStack<typename IsQStack<U>::ItemType>(v);
    else if constexpr (IsQHash<U>::value)      return jsonToHash<T>(v);

    else if constexpr (IsStdVector<U>::value)             return jsonToStdVector<T>(v);
    else if constexpr (IsStdList<U>::value)               return jsonToStdList<T>(v);
    else if constexpr (IsStdDeque<U>::value)              return jsonToStdDeque<T>(v);
    else if constexpr (IsStdSet<U>::value)                return jsonToStdSet<T>(v);
    else if constexpr (IsStdMultiSet<U>::value)           return jsonToStdMultiSet<T>(v);
    else if constexpr (IsStdUnorderedSet<U>::value)       return jsonToStdUnorderedSet<T>(v);
    else if constexpr (IsStdUnorderedMultiSet<U>::value)  return jsonToStdUnorderedMultiSet<T>(v);
    else if constexpr (IsStdMap<U>::value)                return jsonToStdMap<T>(v);
    else if constexpr (IsStdMultiMap<U>::value)           return jsonToStdMultiMap<T>(v);
    // clang-format on

    return T{};
}

} // namespace JsonSerializer
