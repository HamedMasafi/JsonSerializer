
# JsonSerializer

**A modern, type-safe JSON serialization library for Qt 6**

JsonSerializer lets you convert almost any C++ / Qt type to and from `QJsonValue` with just two functions:

```cpp
namespace JsonSerializer {
    template<typename T>
    QJsonValue toJson(const T &value);

    template<typename T>
    T fromJson(const QJsonValue &json);
}
```

---

## Features

- Extremely simple API – only two functions
- Full support for:
  - All basic types registered in `QMetaType`
  - Qt containers (`QList`, `QVector`, `QMap`, `QHash`, `QSet`, `QStack`, `QQueue`, `QMultiMap`, `QMultiHash`, …)
  - STL containers (`std::vector`, `std::list`, `std::deque`, `std::map`, `std::unordered_map`, `std::set`, …)
  - `QObject`-derived classes
  - `Q_GADGET` classes
  - Smart pointers (`QSharedPointer`, `std::shared_ptr`)
  - Custom types via the `Serializable` interface
- Header-only
- Zero macros required for basic usage
- Seamless support for both Qt and modern C++ types

---

## Requirements

- **Qt 6.5+** (tested with Qt 6.5 – 6.8)
- **C++17** or later
- CMake 3.16+

---

## Integration

### CMake (recommended)

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core)

add_subdirectory(JsonSerializer)   # or use FetchContent

target_link_libraries(your_target
    PRIVATE
        JsonSerializer
        Qt6::Core
)
```

### FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
    JsonSerializer
    GIT_REPOSITORY https://github.com/YourUsername/JsonSerializer.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(JsonSerializer)

target_link_libraries(your_target PRIVATE JsonSerializer)
```

---

## Quick Start

```cpp
#include <JsonSerializer/jsonserialize.h>
#include <QDebug>

struct Person {
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(int age MEMBER age)

public:
    QString name;
    int age = 0;
};

int main()
{
    Person p{"Alice", 30};

    // Serialize
    QJsonValue json = JsonSerializer::toJson(p);
    qDebug() << json;           // {"age":30,"name":"Alice"}

    // Deserialize
    Person p2 = JsonSerializer::fromJson<Person>(json);
    qDebug() << p2.name << p2.age;  // Alice 30
}
```

---

## Supported Types

### Basic / QMetaType types
`bool`, `int`, `uint`, `qint64`, `quint64`, `float`, `double`,  
`QString`, `QByteArray`, `QDate`, `QTime`, `QDateTime`,  
`QUrl`, `QUuid`, `QJsonValue`, `QJsonObject`, `QJsonArray`, `QVariant`, …

### Qt Containers

| Container            | Notes                              |
|----------------------|------------------------------------|
| `QList<T>`           |                                    |
| `QVector<T>`         | Treated as `QList` in Qt 6         |
| `QStack<T>`          |                                    |
| `QQueue<T>`          |                                    |
| `QSet<T>`            |                                    |
| `QStringList`        |                                    |
| `QPair<T1,T2>`       |                                    |
| `QMap<Key,T>`        | Object if `Key` is string-like     |
| `QMultiMap<Key,T>`   | Always array of `{key,value}`      |
| `QHash<Key,T>`       | Object if `Key` is string-like     |
| `QMultiHash<Key,T>`  | Always array of `{key,value}`      |

### STL Containers

| Container                          | Notes                              |
|------------------------------------|------------------------------------|
| `std::vector<T>`                   |                                    |
| `std::list<T>`                     |                                    |
| `std::deque<T>`                    |                                    |
| `std::array<T,N>`                  |                                    |
| `std::set` / `std::multiset`       |                                    |
| `std::unordered_set` / `multiset`  |                                    |
| `std::map` / `std::multimap`       | Object if `Key` is string-like     |
| `std::unordered_map` / `multimap`  | Object if `Key` is string-like     |
| `std::stack` / `std::queue`        |                                    |
| `std::pair<T1,T2>`                 |                                    |

### Classes
- Any class inheriting `QObject` (with `Q_OBJECT`)
- Any class marked with `Q_GADGET` + `Q_PROPERTY`
- Classes implementing the `JsonSerializer::Serializable` interface

---

## Map Serialization Rules

- **String-like keys** (`QString`, `std::string`, …) → serialized as a **JSON Object**  
  (cleaner and more natural)
- **Non-string keys** → serialized as a **JSON Array** of objects:
  ```json
  [
    { "key": 1, "value": "one" },
    { "key": 2, "value": "two" }
  ]
  ```
- `QMultiMap` / `QMultiHash` / `std::multimap` / `std::unordered_multimap`  
  are **always** serialized as arrays (because JSON objects cannot have duplicate keys).

---

## Custom Types

### Method 1: Q_GADGET (recommended for value types)

```cpp
struct Point {
    Q_GADGET
    Q_PROPERTY(double x MEMBER x)
    Q_PROPERTY(double y MEMBER y)
public:
    double x = 0.0;
    double y = 0.0;
};
```

### Method 2: Serializable interface

```cpp
class MyClass : public JsonSerializer::Serializable
{
public:
    void saveState(QJsonObject &json) override {
        json["value"] = m_value;
    }

    void restoreState(const QJsonObject &json) override {
        m_value = json["value"].toInt();
    }

private:
    int m_value = 0;
};
```

---


## License

LGPL v3 License

---

## Contributing

Pull requests and issues are welcome!

---

**Made for Qt 6 developers who want clean, modern, and powerful JSON serialization.**
