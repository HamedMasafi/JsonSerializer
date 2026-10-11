#pragma once

#include <QPair>
#include <QStack>
#include <QSet>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "traits.h"
#include "jsonserialize_def.h"

namespace JsonSerializer::Impl {

template<typename T, typename First = typename IsQPair<T>::FirstType, typename Second = typename IsQPair<T>::SecondType>
QPair<First, Second> jsonToPair(const QJsonValue &v)
{
    auto o = v.toObject();
    return qMakePair(fromJson<First>(o["first"]), fromJson<Second>(o["second"]));
}
template<typename T, typename First = typename IsQPair<T>::FirstType, typename Second = typename IsQPair<T>::SecondType>
QJsonValue pairToJson(const QPair<First, Second> &v)
{
    QJsonObject o;
    o["first"] = toJson(v.first);
    o["second"] = toJson(v.second);
    return o;
}

template <typename T>
QList<T> jsonToList(const QJsonValue &v){
    if (!v.isArray())
        return {};
    auto array = v.toArray();
    QList<T> ret;
    for (auto item : std::as_const(array))
        ret << fromJson<T>(item);
    return ret;
}
template <typename T>
QJsonValue listToJson(const QList<T> &v){
    QJsonArray result;

    for (const auto &item : v)
        result.append(toJson(item));

    return result;
}

template <typename T>
QVector<T> jsonToVector(const QJsonValue &v){
    return jsonToList<T>(v);
}
template <typename T>
QJsonValue vectorToJson(const QVector<T> &v)
{
    return listToJson(v);
}

template <typename T>
QQueue<T> jsonToQueue(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsQQueue<T>::ValueType;
    QQueue<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.enqueue(fromJson<ItemType>(item));
    return ret;
}
template <typename T>
QJsonValue queueToJson(const QQueue<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

template <typename T>
QStack<T> jsonToStack(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsQStack<T>::ValueType;
    QStack<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.push(fromJson<ItemType>(item));
    return ret;
}
template <typename T>
QJsonValue stackToJson(const QStack<T> &v)
{
    QJsonArray result;
    for (auto it = v.begin(); v != v.rend(); ++it)
        result.append(toJson(*it));
    return result;
}

template <typename T>
QSet<T> jsonToSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsQSet<T>::ValueType;
    QSet<ItemType> ret;
    ret.reserve(array.size());
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}
template <typename T>
QJsonValue setToJson(const QSet<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

template<typename T, typename KeyType = IsQMap<T>::KeyType, typename ValueType = IsQMap<T>::ValueType>
QMap<KeyType, ValueType> jsonToMap(const QJsonValue &v)
{
    auto arr = v.toArray();
    QMap<KeyType, ValueType> map;
    for (auto item: arr) {
        auto o = item.toObject();
        map.insert(fromJson<KeyType>(o["key"]), fromJson<ValueType>(o["value"]));
    }
    return map;
}
template <typename T, typename KeyType = IsQMap<T>::KeyType, typename ValueType = IsQMap<T>::ValueType>
QJsonValue mapToJson(const QMap<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (auto it = v.begin(); it != v.end(); ++it) {
        QJsonObject o;
        o["key"] = toJson(it.key());
        o["value"] = toJson(it.value());
        arr.append(o);
    }
    return arr;
}

template<typename T, typename KeyType = IsQMultiMap<T>::KeyType, typename ValueType = IsQMultiMap<T>::ValueType>
QMultiMap<KeyType, ValueType> jsonToMultiMap(const QJsonValue &v)
{
    auto arr = v.toArray();
    QMap<KeyType, ValueType> map;
    for (auto item: arr) {
        auto o = item.toObject();
        map.insert(fromJson<KeyType>(o["key"] ),fromJson<ValueType>(o["value"]));
    }
    return map;
}
template <typename T, typename KeyType = IsQMultiMap<T>::KeyType, typename ValueType = IsQMultiMap<T>::ValueType>
QJsonValue multiMapToJson(const QMultiMap<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (auto it = v.begin(); it != v.end(); ++it) {
        QJsonObject o;
        o["key"] = toJson(it.key());
        o["value"] = toJson(it.value());
        arr.append(o);
    }
    return arr;
}

template<typename T, typename KeyType = IsQHash<T>::KeyType, typename ValueType = IsQHash<T>::ValueType>
QHash<KeyType, ValueType> jsonToHash(const QJsonValue &v)
{
    auto arr = v.toArray();
    QHash<KeyType, ValueType> hash;
    for (auto item: arr) {
        auto o = item.toObject();
        hash.insert(fromJson<KeyType>(o["key"] ),fromJson<ValueType>(o["value"]));
    }
    return hash;
}
template <typename T, typename KeyType = IsQHash<T>::KeyType, typename ValueType = IsQHash<T>::ValueType>
QJsonValue hashToJson(const QHash<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (auto it = v.begin(); it != v.end(); ++it) {
        QJsonObject o;
        o["key"] = toJson(it.key());
        o["value"] = toJson(it.value());
        arr.append(o);
    }
    return arr;
}



// ==================== Sequential Containers ====================

// ---------- std::vector ----------
template <typename T>
std::vector<T>  jsonToStdVector(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsStdVector<T>::ValueType;   // یا هر traitی که دارید
    std::vector<ItemType> ret;
    ret.reserve(array.size());
    for (const auto &item : std::as_const(array))
        ret.push_back(fromJson<ItemType>(item));
    return ret;
}

template <typename T>
QJsonValue stdVectorToJson(const std::vector<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ---------- std::list ----------
template <typename T>
std::list<T> jsonToStdList(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsStdList<T>::ValueType;
    std::list<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.push_back(fromJson<ItemType>(item));
    return ret;
}

template <typename T>
QJsonValue stdListToJson(const std::list<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ---------- std::deque ----------
template <typename T>
std::deque<T> jsonToStdDeque(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsStdDeque<T>::ValueType;
    std::deque<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.push_back(fromJson<ItemType>(item));
    return ret;
}

template <typename T>
QJsonValue stdDequeToJson(const std::deque<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ==================== Set Containers ====================

// ---------- std::set ----------
template <typename T>
std::set<T> jsonToStdSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsStdSet<T>::ValueType;
    std::set<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}

template <typename T>
QJsonValue stdSetToJson(const std::set<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ---------- std::multiset ----------
template <typename T>
std::multiset<T> jsonToStdMultiSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsStdMultiSet<T>::ValueType;
    std::multiset<ItemType> ret;
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}

template <typename T>
QJsonValue stdMultiSetToJson(const std::multiset<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ---------- std::unordered_set ----------
template <typename T>
std::unordered_set<T> jsonToStdUnorderedSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsStdUnorderedSet<T>::ValueType;
    std::unordered_set<ItemType> ret;
    ret.reserve(array.size());
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}

template <typename T>
QJsonValue stdUnorderedSetToJson(const std::unordered_set<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ---------- std::unordered_multiset ----------
template <typename T>
std::unordered_multiset<T> jsonToStdUnorderedMultiSet(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto array = v.toArray();
    using ItemType = typename IsStdUnorderedMultiSet<T>::ValueType;
    std::unordered_multiset<ItemType> ret;
    ret.reserve(array.size());
    for (const auto &item : std::as_const(array))
        ret.insert(fromJson<ItemType>(item));
    return ret;
}

template <typename T>
QJsonValue stdUnorderedMultiSetToJson(const std::unordered_multiset<T> &v)
{
    QJsonArray result;
    for (const auto &item : v)
        result.append(toJson(item));
    return result;
}

// ==================== Map Containers (array of {key, value}) ====================

// ---------- std::map ----------
template <typename T,
         typename KeyType   = typename IsStdMap<T>::KeyType,
         typename ValueType = typename IsStdMap<T>::ValueType>
std::map<KeyType, ValueType> jsonToStdMap(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto arr = v.toArray();
    std::map<KeyType, ValueType> map;
    for (const auto &item : std::as_const(arr)) {
        const auto o = item.toObject();
        map.emplace(fromJson<KeyType>(o["key"]),
                    fromJson<ValueType>(o["value"]));
    }
    return map;
}

template <typename T,
         typename KeyType   = typename IsStdMap<T>::KeyType,
         typename ValueType = typename IsStdMap<T>::ValueType>
QJsonValue stdMapToJson(const std::map<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (const auto &pair : v) {
        QJsonObject o;
        o["key"]   = toJson(pair.first);
        o["value"] = toJson(pair.second);
        arr.append(o);
    }
    return arr;
}

// ---------- std::multimap ----------
template <typename T,
         typename KeyType   = typename IsStdMultiMap<T>::KeyType,
         typename ValueType = typename IsStdMultiMap<T>::ValueType>
std::multimap<KeyType, ValueType> jsonToStdMultiMap(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto arr = v.toArray();
    std::multimap<KeyType, ValueType> map;
    for (const auto &item : std::as_const(arr)) {
        const auto o = item.toObject();
        map.emplace(fromJson<KeyType>(o["key"]),
                    fromJson<ValueType>(o["value"]));
    }
    return map;
}

template <typename T,
         typename KeyType   = typename IsStdMultiMap<T>::KeyType,
         typename ValueType = typename IsStdMultiMap<T>::ValueType>
QJsonValue stdMultiMapToJson(const std::multimap<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (const auto &pair : v) {
        QJsonObject o;
        o["key"]   = toJson(pair.first);
        o["value"] = toJson(pair.second);
        arr.append(o);
    }
    return arr;
}

// ---------- std::unordered_map ----------
template <typename T,
         typename KeyType   = typename IsStdUnorderedMap<T>::KeyType,
         typename ValueType = typename IsStdUnorderedMap<T>::ValueType>
std::unordered_map<KeyType, ValueType> jsonToStdUnorderedMap(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto arr = v.toArray();
    std::unordered_map<KeyType, ValueType> map;
    map.reserve(arr.size());
    for (const auto &item : std::as_const(arr)) {
        const auto o = item.toObject();
        map.emplace(fromJson<KeyType>(o["key"]),
                    fromJson<ValueType>(o["value"]));
    }
    return map;
}

template <typename T,
         typename KeyType   = typename IsStdUnorderedMap<T>::KeyType,
         typename ValueType = typename IsStdUnorderedMap<T>::ValueType>
QJsonValue stdUnorderedMapToJson(const std::unordered_map<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (const auto &pair : v) {
        QJsonObject o;
        o["key"]   = toJson(pair.first);
        o["value"] = toJson(pair.second);
        arr.append(o);
    }
    return arr;
}

// ---------- std::unordered_multimap ----------
template <typename T,
         typename KeyType   = typename IsStdUnorderedMultiMap<T>::KeyType,
         typename ValueType = typename IsStdUnorderedMultiMap<T>::ValueType>
std::unordered_multimap<KeyType, ValueType> jsonToStdUnorderedMultiMap(const QJsonValue &v)
{
    if (!v.isArray())
        return {};
    const auto arr = v.toArray();
    std::unordered_multimap<KeyType, ValueType> map;
    map.reserve(arr.size());
    for (const auto &item : std::as_const(arr)) {
        const auto o = item.toObject();
        map.emplace(fromJson<KeyType>(o["key"]),
                    fromJson<ValueType>(o["value"]));
    }
    return map;
}

template <typename T,
         typename KeyType   = typename IsStdUnorderedMultiMap<T>::KeyType,
         typename ValueType = typename IsStdUnorderedMultiMap<T>::ValueType>
QJsonValue stdUnorderedMultiMapToJson(const std::unordered_multimap<KeyType, ValueType> &v)
{
    QJsonArray arr;
    for (const auto &pair : v) {
        QJsonObject o;
        o["key"]   = toJson(pair.first);
        o["value"] = toJson(pair.second);
        arr.append(o);
    }
    return arr;
}

// ==================== std::pair (اختیاری ولی مفید) ====================

template <typename First, typename Second>
std::pair<First, Second> jsonToStdPair(const QJsonValue &v)
{
    if (!v.isObject())
        return {};
    const auto o = v.toObject();
    return {
        fromJson<First>(o["first"]),
        fromJson<Second>(o["second"])
    };
}

template <typename First, typename Second>
QJsonValue stdPairToJson(const std::pair<First, Second> &v)
{
    QJsonObject o;
    o["first"]  = toJson(v.first);
    o["second"] = toJson(v.second);
    return o;
}

} // namespace JsonSerializer::Impl
