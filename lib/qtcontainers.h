#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QPair>
#include <QSet>
#include <QStack>

#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "jsonserialize_def.h"
#include "traits.h"

namespace JsonSerializer::Impl {

template<typename T,
         typename First = typename IsQPair<T>::FirstType,
         typename Second = typename IsQPair<T>::SecondType>
QPair<First, Second> jsonToPair(const QJsonValue &v)
{
    auto o = v.toObject();
    return qMakePair(fromJson<First>(o["first"]), fromJson<Second>(o["second"]));
}
template<typename T,
         typename First = typename IsQPair<T>::FirstType,
         typename Second = typename IsQPair<T>::SecondType>
QJsonValue pairToJson(const QPair<First, Second> &v)
{
    QJsonObject o;
    o["first"] = toJson(v.first);
    o["second"] = toJson(v.second);
    return o;
}

template<typename T, typename ItemType = IsQList<T>::ItemType>
QList<ItemType> jsonToList(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    auto array = v.toArray();
    QList<ItemType> ret;
    for (auto item : std::as_const(array))
        ret << fromJson<ItemType>(item);
    return ret;
}
template<typename T, typename ItemType = IsQList<T>::ItemType>
QJsonValue listToJson(const QList<ItemType> &v)
{
    QJsonArray result;

    for (const auto &item : v)
        result.append(toJson(item));

    return result;
}

template<typename T, typename ItemType = IsQVector<T>::ItemType>
QVector<ItemType> jsonToVector(const QJsonValue &v)
{
    return jsonToList<T>(v);
}
template<typename T>
QJsonValue vectorToJson(const QVector<T> &v)
{
    return listToJson(v);
}

template<typename T, typename ItemType = IsQQueue<T>::ItemType>
QQueue<ItemType> jsonToQueue(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    QQueue<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.enqueue(fromJson<ItemType>(item));
    return ret;
}
template<typename T>
QJsonValue queueToJson(const QQueue<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

template<typename T, typename ItemType = IsQStack<T>::ItemType>
QStack<ItemType> jsonToStack(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    QStack<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.push(fromJson<ItemType>(item));
    return ret;
}
template<typename T>
QJsonValue stackToJson(const QStack<T> &v)
{
    QJsonArray result;
    for (auto it = v.cbegin(); it != v.cend(); ++it)
        result.append(toJson(*it));
    return result;
}

template<typename T, typename ItemType = IsQSet<T>::ItemType>
QSet<ItemType> jsonToSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    QSet<ItemType> ret;
    ret.reserve(array.size());
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}
template<typename T, typename ItemType = IsQSet<T>::ItemType>
QJsonValue setToJson(const QSet<ItemType> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

template<typename T, typename KeyType = IsQMap<T>::KeyType, typename ValueType = IsQMap<T>::ValueType>
QMap<KeyType, ValueType> jsonToMap(const QJsonValue &v)
{
    QMap<KeyType, ValueType> map;
    if constexpr (IsString<KeyType>::value) {
        auto obj = v.toObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if constexpr (std::is_same_v<KeyType, QString>)
                map.insert(it.key(), fromJson<ValueType>(it.value()));
            else
                map.insert(QString::fromStdString(it.key()), fromJson<ValueType>(it.value()));
        }
    } else {
        auto arr = v.toArray();
        for (auto item : arr) {
            auto o = item.toObject();
            map.insert(fromJson<KeyType>(o["key"]), fromJson<ValueType>(o["value"]));
        }
    }
    return map;
}
template<typename T, typename KeyType = IsQMap<T>::KeyType, typename ValueType = IsQMap<T>::ValueType>
QJsonValue mapToJson(const QMap<KeyType, ValueType> &v)
{
    if constexpr (IsString<KeyType>::value) {
        QJsonObject obj;
        for (auto it = v.begin(); it != v.end(); ++it) {
            if constexpr (std::is_same_v<KeyType, QString>)
                obj.insert(it.key(), toJson(it.value()));
            else
                obj.insert(QString::fromStdString(it.key()), toJson(it.value()));
        }
        return obj;
    } else {
        QJsonArray arr;
        for (auto it = v.begin(); it != v.end(); ++it) {
            QJsonObject o;
            o["key"] = toJson(it.key());
            o["value"] = toJson(it.value());
            arr.append(o);
        }
        return arr;
    }
}

// QMultiMap
template<typename T,
         typename KeyType = typename IsQMultiMap<T>::KeyType,
         typename ValueType = typename IsQMultiMap<T>::ValueType>
QMultiMap<KeyType, ValueType> jsonToMultiMap(const QJsonValue &v)
{
    QMultiMap<KeyType, ValueType> map;

    const auto arr = v.toArray();

    for (const auto &item : arr) {
        const auto o = item.toObject();
        map.insert(fromJson<KeyType>(o["key"]), fromJson<ValueType>(o["value"]));
    }

    return map;
}
template<typename T,
         typename KeyType = typename IsQMultiMap<T>::KeyType,
         typename ValueType = typename IsQMultiMap<T>::ValueType>
QJsonValue multiMapToJson(const QMultiMap<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        QJsonObject o;
        o["key"] = toJson(it.key());
        o["value"] = toJson(it.value());
        arr.append(o);
    }
    return arr;
}

// QMultiHash
template<typename T,
         typename KeyType   = typename IsQMultiHash<T>::KeyType,
         typename ValueType = typename IsQMultiHash<T>::ValueType>
QMultiHash<KeyType, ValueType> jsonToMultiHash(const QJsonValue &v)
{
    QMultiHash<KeyType, ValueType> hash;

    const auto arr = v.toArray();
    hash.reserve(arr.size());

    for (const auto &item : arr) {
        const auto o = item.toObject();
        hash.insert(fromJson<KeyType>(o["key"]), fromJson<ValueType>(o["value"]));
    }

    return hash;
}
template<typename T,
         typename KeyType   = typename IsQMultiHash<T>::KeyType,
         typename ValueType = typename IsQMultiHash<T>::ValueType>
QJsonValue multiHashToJson(const QMultiHash<KeyType, ValueType> &hash)
{
    QJsonArray arr;
    for (auto it = hash.constBegin(); it != hash.constEnd(); ++it) {
        QJsonObject o;
        o["key"]   = toJson(it.key());
        o["value"] = toJson(it.value());
        arr.append(o);
    }

    return arr;
}

// QHash
template<typename T,
         typename KeyType = typename IsQHash<T>::KeyType,
         typename ValueType = typename IsQHash<T>::ValueType>
QHash<KeyType, ValueType> jsonToHash(const QJsonValue &v)
{
    QHash<KeyType, ValueType> hash;

    if constexpr (IsString<KeyType>::value) {
        // کلید رشته‌ای → از Object استفاده کن
        const auto obj = v.toObject();
        hash.reserve(obj.size());

        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if constexpr (std::is_same_v<KeyType, QString>) {
                hash.insert(it.key(), fromJson<ValueType>(it.value()));
            } else {
                // مثلاً std::string یا QByteArray
                hash.insert(KeyType{it.key().toStdString()}, fromJson<ValueType>(it.value()));
            }
        }
    } else {
        // کلید غیررشته‌ای → آرایهٔ {key, value}
        const auto arr = v.toArray();
        hash.reserve(arr.size());

        for (const auto &item : arr) {
            const auto o = item.toObject();
            hash.insert(fromJson<KeyType>(o["key"]), fromJson<ValueType>(o["value"]));
        }
    }

    return hash;
}
template<typename T,
         typename KeyType = typename IsQHash<T>::KeyType,
         typename ValueType = typename IsQHash<T>::ValueType>
QJsonValue hashToJson(const QHash<KeyType, ValueType> &v)
{
    if constexpr (IsString<KeyType>::value) {
        QJsonObject obj;
        for (auto it = v.cbegin(); it != v.cend(); ++it) {
            if constexpr (std::is_same_v<KeyType, QString>)
                obj.insert(it.key(), toJson(it.value()));
            else
                obj.insert(QString::fromStdString(it.key()), toJson(it.value()));
        }
        return obj;
    } else {
        QJsonArray arr;
        for (auto it = v.cbegin(); it != v.cend(); ++it) {
            QJsonObject o;
            o["key"] = toJson(it.key());
            o["value"] = toJson(it.value());
            arr.append(o);
        }
        return arr;
    }
}

// ==================== Sequential Containers ====================

// std::vector
template<typename T, typename ItemType = IsStdVector<T>::ItemType>
std::vector<ItemType> jsonToStdVector(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    std::vector<ItemType> ret;
    ret.reserve(array.size());
    for (const auto &item : std::as_const(array))
        ret.push_back(fromJson<ItemType>(item));
    return ret;
}
template<typename T>
QJsonValue stdVectorToJson(const std::vector<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

template<typename T, typename ItemType = IsStdArray<T>::ItemType, std::size_t Size = IsStdArray<T>::Size>
std::array<ItemType, Size> jsonToStdArray(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    std::array<ItemType, Size> arr;
    std::size_t index{};

    const auto array = v.toArray();
    for (const auto &item : std::as_const(array))
        arr[index++] = fromJson<ItemType>(item);
    return arr;
}
template<typename T, std::size_t Size>
QJsonValue stdArrayToJson(const std::array<T, Size> &v)
{
    QJsonArray result;
    for (auto item : v)
        result.append(toJson(item));
    return result;
}

// std::list
template<typename T, typename ItemType = IsStdList<T>::ItemType>
std::list<ItemType> jsonToStdList(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    std::list<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.push_back(fromJson<ItemType>(item));
    return ret;
}

template<typename T>
QJsonValue stdListToJson(const std::list<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// std::deque
template<typename T, typename ItemType = IsStdDeque<T>::ItemType>
std::deque<ItemType> jsonToStdDeque(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    std::deque<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.push_back(fromJson<ItemType>(item));
    return ret;
}

template<typename T>
QJsonValue stdDequeToJson(const std::deque<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ==================== Set Containers ====================

// std::set
template<typename T, typename ItemType = IsStdSet<T>::ItemType>
std::set<ItemType> jsonToStdSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    std::set<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}
template<typename T, typename ItemType = IsStdSet<T>::ItemType>
QJsonValue stdSetToJson(const std::set<ItemType> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// std::multiset
template<typename T, typename ItemType = IsStdMultiSet<T>::ItemType>
std::multiset<ItemType> jsonToStdMultiSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    std::multiset<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}
template<typename T, typename ItemType = IsStdMultiSet<T>::ItemType>
QJsonValue stdMultiSetToJson(const std::multiset<ItemType> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// std::unordered_set
template<typename T, typename ItemType = IsStdUnorderedSet<T>::ItemType>
std::unordered_set<ItemType> jsonToStdUnorderedSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    std::unordered_set<ItemType> ret;
    ret.reserve(array.size());
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}
template<typename T, typename ItemType = IsStdUnorderedSet<T>::ItemType>
QJsonValue stdUnorderedSetToJson(const std::unordered_set<ItemType> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// std::unordered_multiset
template<typename T, typename ItemType = IsStdUnorderedMultiSet<T>::ItemType>
std::unordered_multiset<ItemType> jsonToStdUnorderedMultiSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    std::unordered_multiset<ItemType> ret;
    ret.reserve(array.size());
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}
template<typename T, typename ItemType = IsStdUnorderedMultiSet<T>::ItemType>
QJsonValue stdUnorderedMultiSetToJson(const std::unordered_multiset<ItemType> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ==================== Map Containers (array of {key, value}) ====================

// std::map
template<typename T,
         typename KeyType   = typename IsStdMap<T>::KeyType,
         typename ValueType = typename IsStdMap<T>::ValueType>
std::map<KeyType, ValueType> jsonToStdMap(const QJsonValue &v)
{
    std::map<KeyType, ValueType> map;

    if constexpr (IsString<KeyType>::value) {
        // کلید رشته‌ای → از Object استفاده کن
        if (!v.isObject())
            return map;

        const auto obj = v.toObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if constexpr (std::is_same_v<KeyType, QString>) {
                map.emplace(it.key(), fromJson<ValueType>(it.value()));
            } else if constexpr (std::is_same_v<KeyType, std::string>) {
                map.emplace(it.key().toStdString(), fromJson<ValueType>(it.value()));
            } else {
                // سایر انواع رشته‌ای (مثلاً QByteArray و ...)
                map.emplace(KeyType{it.key().toStdString()}, fromJson<ValueType>(it.value()));
            }
        }
    } else {
        // کلید غیررشته‌ای → آرایهٔ {key, value}
        if (!v.isArray())
            return map;

        const auto arr = v.toArray();
        for (const auto &item : std::as_const(arr)) {
            const auto o = item.toObject();
            map.emplace(fromJson<KeyType>(o["key"]),
                        fromJson<ValueType>(o["value"]));
        }
    }

    return map;
}
template<typename T,
         typename KeyType = typename IsStdMap<T>::KeyType,
         typename ValueType = typename IsStdMap<T>::ValueType>
QJsonValue stdMapToJson(const std::map<KeyType, ValueType> &v)
{
    if constexpr (IsString<KeyType>::value) {
        QJsonObject obj;
        for (const auto &pair : v) {
            if constexpr (std::is_same_v<KeyType, QString>)
                obj.insert(pair.first, toJson(pair.second));
            else
                obj.insert(QString::fromStdString(pair.first), toJson(pair.second));
        }
        return obj;
    } else {
        QJsonArray arr;
        for (const auto &pair : v) {
            QJsonObject o;
            o["key"] = toJson(pair.first);
            o["value"] = toJson(pair.second);
            arr.append(o);
        }
        return arr;
    }
}

// std::multimap
template<typename T,
         typename KeyType = typename IsStdMultiMap<T>::KeyType,
         typename ValueType = typename IsStdMultiMap<T>::ValueType>
std::multimap<KeyType, ValueType> jsonToStdMultiMap(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto arr = v.toArray();
    std::multimap<KeyType, ValueType> map;
    for (const auto &item : std::as_const(arr)) {
        const auto o = item.toObject();
        map.emplace(fromJson<KeyType>(o["key"]), fromJson<ValueType>(o["value"]));
    }
    return map;
}
template<typename T,
         typename KeyType = typename IsStdMultiMap<T>::KeyType,
         typename ValueType = typename IsStdMultiMap<T>::ValueType>
QJsonValue stdMultiMapToJson(const std::multimap<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (const auto &pair : v) {
        QJsonObject o;
        o["key"] = toJson(pair.first);
        o["value"] = toJson(pair.second);
        arr.append(o);
    }
    return arr;
}

// std::unordered_map
template<typename T,
         typename KeyType = typename IsStdUnorderedMap<T>::KeyType,
         typename ValueType = typename IsStdUnorderedMap<T>::ValueType>
std::unordered_map<KeyType, ValueType> jsonToStdUnorderedMap(const QJsonValue &v)
{
    std::unordered_map<KeyType, ValueType> map;

    if constexpr (IsString<KeyType>::value) {
        if (!v.isObject())
            return map;

        const auto obj = v.toObject();
        map.reserve(static_cast<size_t>(obj.size()));

        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if constexpr (std::is_same_v<KeyType, QString>) {
                map.emplace(it.key(), fromJson<ValueType>(it.value()));
            } else if constexpr (std::is_same_v<KeyType, std::string>) {
                map.emplace(it.key().toStdString(), fromJson<ValueType>(it.value()));
            } else {
                // برای انواع رشته‌ای دیگر (QByteArray و ...)
                map.emplace(KeyType{it.key().toStdString()}, fromJson<ValueType>(it.value()));
            }
        }
    } else {
        if (!v.isArray())
            return map;

        const auto arr = v.toArray();
        map.reserve(static_cast<size_t>(arr.size()));

        for (const auto &item : std::as_const(arr)) {
            const auto o = item.toObject();
            map.emplace(fromJson<KeyType>(o["key"]), fromJson<ValueType>(o["value"]));
        }
    }

    return map;
}
template<typename T,
         typename KeyType = typename IsStdUnorderedMap<T>::KeyType,
         typename ValueType = typename IsStdUnorderedMap<T>::ValueType>
QJsonValue stdUnorderedMapToJson(const std::unordered_map<KeyType, ValueType> &v)
{
    if constexpr (IsString<KeyType>::value) {
        QJsonObject obj;
        for (const auto &pair : v) {
            const QString key = [&]() {
                if constexpr (std::is_same_v<KeyType, QString>)
                    return pair.first;
                else
                    return QString::fromStdString(pair.first);
            }();

            obj.insert(key, toJson(pair.second));
        }
        return obj;
    } else {
        QJsonArray arr;
        for (const auto &pair : v) {
            QJsonObject o;
            o["key"] = toJson(pair.first);
            o["value"] = toJson(pair.second);
            arr.append(o);
        }
        return arr;
    }
}

// std::unordered_multimap
template<typename T,
         typename KeyType   = typename IsStdUnorderedMultiMap<T>::KeyType,
         typename ValueType = typename IsStdUnorderedMultiMap<T>::ValueType>
std::unordered_multimap<KeyType, ValueType> jsonToStdUnorderedMultiMap(const QJsonValue &v)
{
    std::unordered_multimap<KeyType, ValueType> map;

    if (!v.isArray())
        return map;

    const auto arr = v.toArray();

    for (const auto &item : std::as_const(arr)) {
        const auto o = item.toObject();
        map.emplace(fromJson<KeyType>(o["key"]), fromJson<ValueType>(o["value"]));
    }

    return map;
}
template<typename T,
         typename KeyType = typename IsStdUnorderedMultiMap<T>::KeyType,
         typename ValueType = typename IsStdUnorderedMultiMap<T>::ValueType>
QJsonValue stdUnorderedMultiMapToJson(const std::unordered_multimap<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (const auto &pair : v) {
        QJsonObject o;
        o["key"] = toJson(pair.first);
        o["value"] = toJson(pair.second);
        arr.append(o);
    }
    return arr;
}

// ==================== std::pair (اختیاری ولی مفید) ====================

template<typename First, typename Second>
std::pair<First, Second> jsonToStdPair(const QJsonValue &v)
{
    if (!v.isObject())
        return {};
    const auto o = v.toObject();
    return {fromJson<First>(o["first"]), fromJson<Second>(o["second"])};
}

template<typename First, typename Second>
QJsonValue stdPairToJson(const std::pair<First, Second> &v)
{
    QJsonObject o;
    o["first"] = toJson(v.first);
    o["second"] = toJson(v.second);
    return o;
}

} // namespace JsonSerializer::Impl
