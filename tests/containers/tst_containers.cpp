#include <QtTest>
#include <jsonserialize.h>

#include <QHash>
#include <QList>
#include <QMap>
#include <QMultiHash>
#include <QMultiMap>
#include <QPair>
#include <QQueue>
#include <QSet>
#include <QStack>
#include <QVector>

#include <array>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace JsonSerializer;

template<typename MultiMap>
bool multiMapEqualIgnoreOrder(const MultiMap &a, const MultiMap &b)
{
    if (a.size() != b.size())
        return false;

    // همه کلیدهای یکتا را جمع می‌کنیم
    QSet<typename MultiMap::key_type> keys;
    for (auto it = a.constBegin(); it != a.constEnd(); ++it)
        keys.insert(it.key());
    for (auto it = b.constBegin(); it != b.constEnd(); ++it)
        keys.insert(it.key());

    for (const auto &key : keys) {
        auto valuesA = a.values(key); // QList
        auto valuesB = b.values(key);

        if (valuesA.size() != valuesB.size())
            return false;

        // مرتب می‌کنیم تا ترتیب بی‌اهمیت شود
        std::sort(valuesA.begin(), valuesA.end());
        std::sort(valuesB.begin(), valuesB.end());

        if (valuesA != valuesB)
            return false;
    }
    return true;
}

class ContainersTest : public QObject
{
    Q_OBJECT

private slots:
    // ---------- Sequence (Qt) ----------
    void testQList();
    void testQVector();
    void testQQueue();
    void testQStack();

    // ---------- Set-like (Qt) ----------
    void testQSet();

    // ---------- Pair ----------
    void testQPair();

    // ---------- Map-like (Qt) ----------
    void testQMap_StringKey();
    void testQMap_IntKey();
    void testQMultiMap_StringKey();
    void testQMultiMap_IntKey();
    void testQHash_StringKey();
    void testQHash_IntKey();
    void testQMultiHash_StringKey();
    void testQMultiHash_IntKey();

    // ---------- Sequence (std) ----------
    void testStdArray();
    void testStdVector();
    void testStdList();
    void testStdDeque();

    // ---------- Set-like (std) ----------
    void testStdSet();
    void testStdMultiSet();
    void testStdUnorderedSet();
    void testStdUnorderedMultiSet();

    // ---------- Map-like (std) ----------
    void testStdMap_StringKey();
    void testStdMap_IntKey();
    void testStdMultiMap_StringKey();
    void testStdMultiMap_IntKey();
};

// ============================================================
// QList
// ============================================================
void ContainersTest::testQList()
{
    QList<int> original{10, 20, 30, 40, 50};

    QJsonValue json = toJson(original);
    auto restored = fromJson<QList<int>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// QVector
// ============================================================
void ContainersTest::testQVector()
{
    QVector<QString> original{"alpha", "beta", "gamma"};

    QJsonValue json = toJson(original);
    auto restored = fromJson<QVector<QString>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// QQueue
// ============================================================
void ContainersTest::testQQueue()
{
    QQueue<int> original;
    original.enqueue(1);
    original.enqueue(2);
    original.enqueue(3);

    QJsonValue json = toJson(original);
    auto restored = fromJson<QQueue<int>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// QStack
// ============================================================
void ContainersTest::testQStack()
{
    QStack<QString> original;
    original.push("first");
    original.push("second");
    original.push("third");

    QJsonValue json = toJson(original);
    auto restored = fromJson<QStack<QString>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// QSet
// ============================================================
void ContainersTest::testQSet()
{
    QSet<int> original{5, 10, 15, 20, 25};

    QJsonValue json = toJson(original);
    auto restored = fromJson<QSet<int>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// QPair
// ============================================================
void ContainersTest::testQPair()
{
    QPair<QString, int> original{"answer", 42};

    QJsonValue json = toJson(original);
    auto restored = fromJson<QPair<QString, int>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// QMap
// ============================================================
void ContainersTest::testQMap_StringKey()
{
    QMap<QString, int> original{{"apple", 10}, {"banana", 20}, {"orange", 15}};

    QJsonValue json = toJson(original);
    auto restored = fromJson<QMap<QString, int>>(json);

    QCOMPARE(original, restored);
}

void ContainersTest::testQMap_IntKey()
{
    QMap<int, QString> original{{1, "one"}, {2, "two"}, {3, "three"}};

    QJsonValue json = toJson(original);
    auto restored = fromJson<QMap<int, QString>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// QMultiMap
// ============================================================
void ContainersTest::testQMultiMap_IntKey()
{
    QMultiMap<int, int> original;
    original.insert(1, 100);
    original.insert(1, 200);
    original.insert(2, 300);
    original.insert(2, 400);
    original.insert(1, 150);

    QJsonValue json = toJson(original);
    auto restored = fromJson<QMultiMap<int, int>>(json);

    QVERIFY2(multiMapEqualIgnoreOrder(restored, original),
             "QMultiMap<int,int> content mismatch (order ignored)");
}

void ContainersTest::testQMultiMap_StringKey()
{
    QMultiMap<QString, QString> original;
    original.insert("color", "red");
    original.insert("color", "blue");
    original.insert("size", "large");
    original.insert("size", "medium");
    original.insert("color", "green");

    QJsonValue json = toJson(original);
    auto restored = fromJson<QMultiMap<QString, QString>>(json);

    QVERIFY2(multiMapEqualIgnoreOrder(original, restored),
             "QMultiMap<QString,QString> content mismatch (order ignored)");
}

// ============================================================
// QHash
// ============================================================
void ContainersTest::testQHash_StringKey()
{
    QHash<QString, double> original{{"pi", 3.14159}, {"e", 2.71828}};

    QJsonValue json = toJson(original);
    auto restored = fromJson<QHash<QString, double>>(json);

    QCOMPARE(original, restored);
}

void ContainersTest::testQHash_IntKey()
{
    QHash<int, bool> original{{10, true}, {20, false}, {30, true}};

    QJsonValue json = toJson(original);
    auto restored = fromJson<QHash<int, bool>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// QMultiHash
// ============================================================
void ContainersTest::testQMultiHash_StringKey()
{
    QMultiHash<QString, int> original;
    original.insert("tag", 1);
    original.insert("tag", 2);
    original.insert("tag", 3);
    original.insert("level", 99);

    QJsonValue json = toJson(original);
    auto restored = fromJson<QMultiHash<QString, int>>(json);

    QCOMPARE(original, restored);
}

void ContainersTest::testQMultiHash_IntKey()
{
    QMultiHash<int, QString> original;
    original.insert(5, "five-a");
    original.insert(5, "five-b");
    original.insert(7, "seven");
    original.insert(7, "seven-b");

    QJsonValue json = toJson(original);
    auto restored = fromJson<QMultiHash<int, QString>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::array
// ============================================================
void ContainersTest::testStdArray()
{
    std::array<int, 4> original{11, 22, 33, 44};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::array<int, 4>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::vector
// ============================================================
void ContainersTest::testStdVector()
{
    std::vector<std::string> original{"hello", "world", "test"};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::vector<std::string>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::list
// ============================================================
void ContainersTest::testStdList()
{
    std::list<int> original{100, 200, 300, 400};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::list<int>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::deque
// ============================================================
void ContainersTest::testStdDeque()
{
    std::deque<QString> original;
    original.push_back("front");
    original.push_back("middle");
    original.push_back("back");

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::deque<QString>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::set
// ============================================================
void ContainersTest::testStdSet()
{
    std::set<int> original{5, 10, 15, 20, 25};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::set<int>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::multiset
// ============================================================
void ContainersTest::testStdMultiSet()
{
    std::multiset<int> original{1, 2, 2, 3, 3, 3};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::multiset<int>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::unordered_set
// ============================================================
void ContainersTest::testStdUnorderedSet()
{
    std::unordered_set<std::string> original{"red", "green", "blue"};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::unordered_set<std::string>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::unordered_multiset
// ============================================================
void ContainersTest::testStdUnorderedMultiSet()
{
    std::unordered_multiset<int> original{7, 7, 8, 9, 9, 9};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::unordered_multiset<int>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::map
// ============================================================
void ContainersTest::testStdMap_StringKey()
{
    std::map<std::string, int> original{{"apple", 10}, {"banana", 20}, {"orange", 15}};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::map<std::string, int>>(json);

    QCOMPARE(original, restored);
}

void ContainersTest::testStdMap_IntKey()
{
    std::map<int, std::string> original{{1, "one"}, {2, "two"}, {3, "three"}};

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::map<int, std::string>>(json);

    QCOMPARE(original, restored);
}

// ============================================================
// std::multimap
// ============================================================
void ContainersTest::testStdMultiMap_StringKey()
{
    std::multimap<std::string, int> original;
    original.emplace("color", 1);
    original.emplace("color", 2);
    original.emplace("size", 3);
    original.emplace("size", 4);
    original.emplace("color", 5);

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::multimap<std::string, int>>(json);

    // برای std::multimap هم می‌توانیم از همان منطق استفاده کنیم
    // یا به صورت دستی:

    QCOMPARE(restored.size(), original.size());

    // کلیدهای یکتا
    std::set<std::string> keys;
    for (const auto &p : original)
        keys.insert(p.first);
    for (const auto &p : restored)
        keys.insert(p.first);

    for (const auto &key : keys) {
        auto rangeA = original.equal_range(key);
        auto rangeB = restored.equal_range(key);

        std::vector<int> valuesA, valuesB;
        for (auto it = rangeA.first; it != rangeA.second; ++it)
            valuesA.push_back(it->second);
        for (auto it = rangeB.first; it != rangeB.second; ++it)
            valuesB.push_back(it->second);

        std::sort(valuesA.begin(), valuesA.end());
        std::sort(valuesB.begin(), valuesB.end());

        QCOMPARE(valuesA, valuesB);
    }
}

void ContainersTest::testStdMultiMap_IntKey()
{
    std::multimap<int, std::string> original;
    original.emplace(1, "a");
    original.emplace(1, "b");
    original.emplace(2, "c");
    original.emplace(2, "d");

    QJsonValue json = toJson(original);
    auto restored = fromJson<std::multimap<int, std::string>>(json);

    QCOMPARE(original, restored);
}

QTEST_MAIN(ContainersTest)
#include "tst_containers.moc"