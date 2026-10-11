#pragma once

#include <QMultiMap>
#include <QSharedPointer>

#include <deque>
#include <list>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace JsonSerializer::Impl {

using namespace std;

#define IsContainer(type) \
    template<typename T> \
    struct Is##type : std::false_type \
    {}; \
    template<typename T> \
    struct Is##type<type<T>> : std::true_type \
    { \
        using ItemType = T; \
    };

#define IsContainerX(type, name) \
    template<typename T> \
    struct Is##name : std::false_type \
    {}; \
    template<typename T> \
    struct Is##name<type<T>> : std::true_type \
    { \
        using ItemType = T; \
    };

#define IsKeyValueContainer(type) \
    template<typename T> \
    struct Is##type : std::false_type \
    {}; \
    template<typename _KeyType, typename _ValueType> \
    struct Is##type<type<_KeyType, _ValueType>> : std::true_type \
    { \
        using KeyType = _KeyType; \
        using ValueType = _ValueType; \
    };

#define IsKeyValueContainerX(type, name) \
    template<typename T> \
    struct Is##name : std::false_type \
    {}; \
    template<typename _KeyType, typename _ValueType> \
    struct Is##name<type<_KeyType, _ValueType>> : std::true_type \
    { \
        using KeyType = _KeyType; \
        using ValueType = _ValueType; \
    };

// clang-format off
IsContainer(QQueue)
IsContainer(QStack)
IsContainer(QSet)
IsContainer(QList)
IsContainer(QVector)
IsContainerX(std::vector, StdVector)
IsContainerX(std::list, StdList)
IsContainerX(std::deque, StdDeque)
IsContainerX(std::set, StdSet)
IsContainerX(std::multiset, StdMultiSet)
IsContainerX(std::unordered_set, StdUnorderedSet)
IsContainerX(std::unordered_multiset, StdUnorderedMultiSet)

IsKeyValueContainer(QMap)
IsKeyValueContainer(QMultiMap)
IsKeyValueContainer(QHash)
IsKeyValueContainer(QMultiHash)
IsKeyValueContainerX(std::map, StdMap)
IsKeyValueContainerX(std::multimap, StdMultiMap)

    // clang-format on

    // QPair
    template<typename T>
    struct IsQPair : std::false_type
{};

template<typename First, typename Second>
struct IsQPair<QPair<First, Second>> : std::true_type
{
    using FirstType = First;
    using SecondType = Second;
};

// QSharedPointer
template<typename T>
struct IsQSharedPointer : std::false_type
{};
template<typename T>
struct IsQSharedPointer<QSharedPointer<T>> : std::true_type
{
    using DataType = T;
};

// std::shared_ptr
template<typename T>
struct IsStdPointer : std::false_type
{};
template<typename T>
struct IsStdPointer<std::shared_ptr<T>> : std::true_type
{
    using DataType = T;
};

template<typename T, typename = void>
struct is_qgadget : std::false_type
{};

template<typename T>
struct is_qgadget<T, std::void_t<typename T::QtGadgetHelper>> : std::true_type
{};

template<typename T>
constexpr bool IsQGadget = is_qgadget<std::remove_cv_t<std::remove_pointer_t<T>>>::value;

} // namespace JsonSerializer::Impl