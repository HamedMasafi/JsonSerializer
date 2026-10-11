# JsonSerializer

**A modern, header-only, type-safe JSON serialization library for Qt 6**

JsonSerializer provides a clean and powerful way to convert almost any C++ / Qt type to and from `QJsonValue` with a single pair of functions:

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

- **Extremely simple API** – just two functions
- Full support for:
  - All basic types registered in `QMetaType`
  - Qt containers (`QList`, `QVector`, `QMap`, `QHash`, `QSet`, `QStack`, `QQueue`, `QMultiMap`, `QMultiHash`, ...)
  - STL containers (`std::vector`, `std::list`, `std::deque`, `std::map`, `std::unordered_map`, `std::set`, `std::unordered_set`, ...)
  - `QObject` derived classes
  - `Q_GADGET` classes
  - Smart pointers (`QSharedPointer`, `std::shared_ptr`)
  - Custom types via `Serializable` interface
- Header-only
- Zero macros required for basic usage
- Works with both Qt and modern C++ types seamlessly

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

### FetchContent example

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
    qDebug() << json;   // {"age":30,"name":"Alice"}

    // Deserialize
    Person p2 = JsonSerializer::fromJson<Person>(json);
    qDebug() << p2.name << p2.age;
}
```

---

## Supported Types

### Basic / QMetaType types
`bool`, `int`, `uint`, `qint64`, `quint64`, `float`, `double`, `QString`, `QByteArray`, `QDate`, `QTime`, `QDateTime`, `QUrl`, `QUuid`, `QJsonValue`, `QJsonObject`, `QJsonArray`, `QVariant`, ...

### Qt Containers

| Container             | Supported |
|-----------------------|-----------|
| `QList<T>`            | ✅        |
| `QVector<T>`          | ✅        |
| `QStack<T>`           | ✅        |
| `QQueue<T>`           | ✅        |
| `QSet<T>`             | ✅        |
| `QMap<Key, T>`        | ✅        |
| `QMultiMap<Key, T>`   | ✅        |
| `QHash<Key, T>`       | ✅        |
| `QMultiHash<Key, T>`  | ✅        |
| `QStringList`         | ✅        |
| `QPair<T1, T2>`       | ✅        |

### STL Containers

| Container                             | Supported |
|---------------------------------------|-----------|
| `std::vector<T>`                      | ✅        |
| `std::list<T>`                        | ✅        |
| `std::deque<T>`                       | ✅        |
| `std::array<T, N>`                    | ✅        |
| `std::set<T>` / `std::multiset`       | ✅        |
| `std::unordered_set` / `multiset`     | ✅        |
| `std::map` / `std::multimap`          | ✅        |
| `std::unordered_map` / `multimap`     | ✅        |
| `std::stack` / `std::queue`           | ✅        |
| `std::pair<T1, T2>`                   | ✅        |

### Classes
- Any class inheriting `QObject` (with `Q_OBJECT`)
- Any class marked with `Q_GADGET` + `Q_PROPERTY`
- Classes implementing the `JsonSerializer::Serializable` interface

---

## Custom Types

### Method 1: Q_GADGET (recommended for value types)

```cpp
struct Point {
    Q_GADGET
    Q_PROPERTY(double x MEMBER x)
    Q_PROPERTY(double y MEMBER y)
public:
    double x = 0, y = 0;
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

## Notes & Design Decisions

- Maps (`QMap`, `std::map`, etc.) are serialized as **JSON arrays of objects** with `"key"` and `"value"` fields. This allows non-string keys.
- `QVector` is treated as an alias of `QList` (as it is in Qt 6).
- Pointers (`T*`, `QSharedPointer<T>`, `std::shared_ptr<T>`) are supported (ownership is transferred on deserialization for raw pointers).
- The library is **header-only**. Just include `jsonserialize.h`.

---

## License

MIT License – see [LICENSE](LICENSE) for details.

---

## Contributing

Pull requests and issues are welcome!

---

**Made for Qt 6 developers who want clean, modern, and powerful JSON serialization.**