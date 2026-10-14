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

    if constexpr (std::is_null_pointer_v<U>)
        return {};
    else if constexpr (Impl::IsQtTypeType<U>::value)
        return basicTypeToJson(v);
    else if constexpr (std::is_base_of_v<QObject, U>)
        return objectToJson(v);
    else if constexpr (Impl::IsQGadget<U>)
        return gadgetToJson(v, U::staticMetaObject);
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

    else if constexpr (IsQPair<U>::value)      return pairToJson<U>(v);
    else if constexpr (IsQList<U>::value)      return listToJson<U>(v);
    else if constexpr (IsQVector<U>::value)    return vectorToJson<U>(v);
    else if constexpr (IsQMap<U>::value)       return mapToJson<U>(v);
    else if constexpr (IsQMultiMap<U>::value)  return multiMapToJson<U>(v);
    else if constexpr (IsQStack<U>::value)     return stackToJson(v);
    else if constexpr (IsQQueue<U>::value)     return queueToJson(v);
    else if constexpr (IsQSet<U>::value)       return setToJson<U>(v);
    else if constexpr (IsQHash<U>::value)      return hashToJson<U>(v);
    else if constexpr (IsQMultiHash<U>::value) return multiHashToJson<U>(v);

    else if constexpr (IsStdArray<U>::value)              return stdArrayToJson(v);
    else if constexpr (IsStdVector<U>::value)             return stdVectorToJson(v);
    else if constexpr (IsStdList<U>::value)               return stdListToJson(v);
    else if constexpr (IsStdDeque<U>::value)              return stdDequeToJson(v);
    else if constexpr (IsStdSet<U>::value)                return stdSetToJson<U>(v);
    else if constexpr (IsStdMultiSet<U>::value)           return stdMultiSetToJson<U>(v);
    else if constexpr (IsStdUnorderedSet<U>::value)       return stdUnorderedSetToJson<U>(v);
    else if constexpr (IsStdUnorderedMultiSet<U>::value)  return stdUnorderedMultiSetToJson<U>(v);
    else if constexpr (IsStdMap<U>::value)                return stdMapToJson<U>(v);
    else if constexpr (IsStdMultiMap<U>::value)           return stdMultiMapToJson<U>(v);

    // clang-format on

    return QJsonValue::fromVariant(QVariant::fromValue(v));
}

template<typename T>
T fromJson(const QJsonValue &v)
{
    using namespace Impl;
    using Raw = std::remove_cv_t<std::remove_reference_t<T>>;
    using U = std::remove_pointer_t<Raw>; // حالا U همیشه خود کلاس است

    if constexpr (std::is_null_pointer_v<U>)
        return nullptr;
    else if constexpr (Impl::IsQtTypeType<U>::value) {
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
    else if constexpr (IsQList<U>::value)      return jsonToList<T>(v);
    else if constexpr (IsQVector<U>::value)    return jsonToVector<T>(v);
    else if constexpr (IsQSet<U>::value)       return jsonToSet<T>(v);
    else if constexpr (IsQQueue<U>::value)     return jsonToQueue<U>(v);
    else if constexpr (IsQPair<U>::value)      return jsonToPair<T>(v);
    else if constexpr (IsQMap<U>::value)       return jsonToMap<T>(v);
    else if constexpr (IsQMultiMap<U>::value)  return jsonToMultiMap<T>(v);
    else if constexpr (IsQStack<U>::value)     return jsonToStack<U>(v);
    else if constexpr (IsQHash<U>::value)      return jsonToHash<T>(v);
    else if constexpr (IsQMultiHash<U>::value) return jsonToMultiHash<T>(v);

    else if constexpr (IsStdArray<U>::value)              return jsonToStdArray<T>(v);
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
