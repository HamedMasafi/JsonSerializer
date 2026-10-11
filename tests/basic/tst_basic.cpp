#include <QBitArray>
#include <QBitmap>
#include <QBrush>
#include <QColor>
#include <QColorSpace>
#include <QDateTime>
#include <QEasingCurve>
#include <QFont>
#include <QIcon>
#include <QImage>
#include <QKeySequence>
#include <QPalette>
#include <QPen>
#include <QPixmap>
#include <QPolygon>
#include <QPolygonF>
#include <QRegion>
#include <QRegularExpression>
#include <QTextFormat>
#include <QTransform>
#include <QUrl>
#include <QUuid>
#include <QtTest>

#include <jsonserialize.h>

#define printJson(x) qDebug() << JsonSerializer::toJson(x);
#define TEST(value, Type) \
    do { \
        const auto json_##value = toJson(value); \
        const auto result_##value = fromJson<Type>(json_##value); \
        QCOMPARE(result_##value, value); \
    } while (false)

QPixmap createRandomPixmap(const QSize &size)
{
    QImage image{size, QImage::Format_ARGB32};

    auto *rng = QRandomGenerator::global();

    for (int y = 0; y < image.height(); ++y) {
        auto *line = reinterpret_cast<QRgb *>(image.scanLine(y));

        for (int x = 0; x < image.width(); ++x) {
            line[x] = qRgba(
                rng->bounded(256),
                rng->bounded(256),
                rng->bounded(256),
                rng->bounded(256)
                );
        }
    }

    return QPixmap::fromImage(image);
}

bool comparePixmaps(const QPixmap &a, const QPixmap &b)
{
    if (a.size() != b.size())
        return false;

    for (int y = 0; y < a.height(); ++y) {
        for (int x = 0; x < a.width(); ++x) {
            if (a.toImage().pixel(x, y) != b.toImage().pixel(x, y))
                return false;
        }
    }

    return true;
}

class MyGadget
{
    Q_GADGET

    Q_PROPERTY(int intValue READ intValue WRITE setIntValue)
    Q_PROPERTY(QString stringValue READ stringValue WRITE setStringValue)
    Q_PROPERTY(double doubleValue READ doubleValue WRITE setDoubleValue)

public:
    int intValue() const;
    void setIntValue(int newIntValue);

    QString stringValue() const;
    void setStringValue(const QString &newStringValue);

    double doubleValue() const;
    void setDoubleValue(double newDoubleValue);

private:
    int m_intValue;
    QString m_stringValue;
    double m_doubleValue;
};

class BasicTest : public QObject
{
    Q_OBJECT

private slots:
    void testBasicTypes();
    void testSharedPointer();
    void testStringList();
    void testMap();
    void testPair();
    void testGadget();
    void testAllVectorLikeContainers();
};

void BasicTest::testBasicTypes()
{
    using namespace JsonSerializer;

    // Primitive types
    bool boolValue = true;
    int intValue = -12345;
    uint uintValue = 12345U;
    qlonglong longLongValue = -1234567890123LL;
    qulonglong ulongLongValue = 1234567890123ULL;
    double doubleValue = 123.456789;
    long longValue = -123456L;
    short shortValue = -1234;
    char charValue = 'A';
    ulong ulongValue = 123456UL;
    ushort ushortValue = 1234U;
    uchar ucharValue = 255U;
    float floatValue = 12.5F;
    signed char scharValue = -100;

    TEST(boolValue, bool);
    TEST(intValue, int);
    TEST(uintValue, uint);
    TEST(longLongValue, qlonglong);
    TEST(ulongLongValue, qulonglong);
    TEST(doubleValue, double);
    TEST(longValue, long);
    TEST(shortValue, short);
    TEST(charValue, char);
    TEST(ulongValue, ulong);
    TEST(ushortValue, ushort);
    TEST(ucharValue, uchar);
    TEST(floatValue, float);
    TEST(scharValue, signed char);

    // Strings and byte arrays
    QString stringValue = QStringLiteral("Qt Serializer تست");
    QStringList stringListValue{QStringLiteral("alpha"),
                                QStringLiteral("beta"),
                                QStringLiteral("فارسی")};
    QByteArray byteArrayValue = QByteArray::fromHex("00ff10207f");
    QDate dateValue{2026, 10, 11};
    QTime timeValue{14, 35, 42, 123};
    QDateTime dateTimeValue{QDate{2026, 10, 11}, QTime{14, 35, 42, 123}, QTimeZone::UTC};
    QUrl urlValue{QStringLiteral("https://example.com/path?q=qt")};

    TEST(stringValue, QString);
    TEST(stringListValue, QStringList);
    TEST(byteArrayValue, QByteArray);
    TEST(dateValue, QDate);
    TEST(timeValue, QTime);
    TEST(dateTimeValue, QDateTime);
    TEST(urlValue, QUrl);

    // Geometry
    QRect rectValue{10, 20, 300, 200};
    QRectF rectFValue{10.5, 20.25, 300.75, 200.125};
    QSize sizeValue{640, 480};
    QSizeF sizeFValue{640.5, 480.25};
    QLine lineValue{1, 2, 30, 40};
    QLineF lineFValue{1.5, 2.5, 30.5, 40.5};
    QPoint pointValue{100, 200};
    QPointF pointFValue{100.5, 200.25};

    TEST(rectValue, QRect);
    TEST(rectFValue, QRectF);
    TEST(sizeValue, QSize);
    TEST(sizeFValue, QSizeF);
    TEST(lineValue, QLine);
    TEST(lineFValue, QLineF);
    TEST(pointValue, QPoint);
    TEST(pointFValue, QPointF);

    // UUID and graphics
    QUuid uuidValue{QStringLiteral("{12345678-1234-5678-9abc-def012345678}")};
    QColor colorValue{23, 120, 210, 180};

    QFont fontValue{QStringLiteral("Arial"), 12, QFont::DemiBold};
    fontValue.setItalic(true);

    QImage imageValue{8, 8, QImage::Format_ARGB32};
    imageValue.fill(QColor{10, 20, 30, 255});

    QPixmap pixmapValue = createRandomPixmap(QSize{20, 20});
    const auto jsonPixmapValue = toJson(pixmapValue);
    const auto resultPixmapValue = fromJson<QPixmap>(jsonPixmapValue);
    QVERIFY(comparePixmaps(resultPixmapValue, pixmapValue));

    QIcon iconValue{pixmapValue};

    QBrush brushValue{QColor{10, 20, 30}, Qt::Dense4Pattern};

    QPen penValue{QColor{200, 50, 80}};
    penValue.setWidthF(2.5);
    penValue.setStyle(Qt::DashLine);

    QPalette paletteValue;
    paletteValue.setColor(QPalette::Window, QColor{40, 50, 60});
    paletteValue.setColor(QPalette::Text, QColor{220, 230, 240});

    QBitArray bitArrayValue{8, false};
    bitArrayValue.setBit(0);
    bitArrayValue.setBit(3);
    bitArrayValue.setBit(7);

    QPolygon polygonValue{QPoint{0, 0}, QPoint{10, 0}, QPoint{10, 10}, QPoint{0, 10}};

    QPolygonF polygonFValue{QPointF{0.5, 0.5}, QPointF{10.5, 0.5}, QPointF{10.5, 10.5}};

    QRegion regionValue{QRect{0, 0, 100, 100}};
    regionValue -= QRegion{QRect{20, 20, 30, 30}};

    QBitmap bitmapValue{QBitmap::fromImage(imageValue)};

    QKeySequence keySequenceValue{QKeySequence::Save};

    QTextFormat textFormatValue{QTextFormat::CharFormat};
    textFormatValue.setProperty(QTextFormat::UserProperty, 42);

    QTransform transformValue;
    transformValue.translate(10.5, 20.5);
    transformValue.rotate(30.0);
    transformValue.scale(2.0, 3.0);

    QRegularExpression regularExpressionValue{QStringLiteral(R"(\b[A-Z]{2,5}\d+\b)")};
    regularExpressionValue.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

    QEasingCurve easingCurveValue{QEasingCurve::OutBounce};

    QByteArrayList byteArrayListValue{QByteArray{"alpha"},
                                      QByteArray::fromHex("00ff"),
                                      QByteArray{"omega"}};

    QColorSpace colorSpaceValue{QColorSpace::SRgb};

    TEST(uuidValue, QUuid);
    TEST(colorValue, QColor);
    TEST(fontValue, QFont);
    TEST(imageValue, QImage);
    // TEST(iconValue, QIcon);
    TEST(brushValue, QBrush);
    TEST(penValue, QPen);
    TEST(paletteValue, QPalette);
    TEST(bitArrayValue, QBitArray);
    TEST(polygonValue, QPolygon);
    TEST(polygonFValue, QPolygonF);
    TEST(regionValue, QRegion);
    // TEST(bitmapValue, QBitmap);
    TEST(keySequenceValue, QKeySequence);
    // TEST(textFormatValue, QTextFormat);
    TEST(transformValue, QTransform);
    TEST(regularExpressionValue, QRegularExpression);
    TEST(easingCurveValue, QEasingCurve);
    TEST(byteArrayListValue, QByteArrayList);
    TEST(colorSpaceValue, QColorSpace);
}

void BasicTest::testSharedPointer()
{
    using namespace JsonSerializer;
    QSharedPointer<MyGadget> c{new MyGadget};
    c->setIntValue(1234);
    c->setStringValue("Hello world!");
    c->setDoubleValue(3.14);

    auto json = toJson(c);
    auto c2 = fromJson<QSharedPointer<MyGadget>>(json);
    QCOMPARE(c->intValue(), c2->intValue());
    QCOMPARE(c->stringValue(), c2->stringValue());
    QCOMPARE(c->doubleValue(), c2->doubleValue());
}

void BasicTest::testStringList()
{
    using namespace JsonSerializer;
    QStringList list{"One", "Two", "Three"};
    qDebug() << toJson(list);

    auto json = toJson(list);
    auto list2 = fromJson<QStringList>(json);
    QCOMPARE(list, list2);
}

void BasicTest::testMap()
{
    using namespace JsonSerializer;
    QMap<QString, int> map;
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);
    qDebug() << toJson(map);

    auto mapJson = toJson(map);
    auto mapCopy = fromJson<QMap<QString, int>>(mapJson);

    QCOMPARE(map, mapCopy);

    QVariantMap map2;
    map2.insert("name", "hamed");
    map2.insert("int", 1234);
    map2.insert("pi", 3.14);
    map2.insert("ch", 'a');
    auto json2 = toJson(map2);
    auto map3 = fromJson<QVariantMap>(json2);
    QCOMPARE(map2, map3);
}

void BasicTest::testPair()
{
    using namespace JsonSerializer;

    auto pair = qMakePair(1, 2);
    auto pairJson = toJson(pair);
    auto pairCopy = fromJson<QPair<int, int>>(pairJson);
    QCOMPARE(pair, pairCopy);

    auto pair2 = qMakePair(1, QString{"2"});
    auto pair2Json = toJson(pair2);
    auto pair2Copy = fromJson<QPair<int, QString>>(pair2Json);
    QCOMPARE(pair2, pair2Copy);
}

void BasicTest::testGadget()
{
    MyGadget *c = new MyGadget;
    c->setIntValue(1234);
    c->setStringValue("Hello world!");
    c->setDoubleValue(3.14);
    auto json = JsonSerializer::toJson(c);
    qDebug() << json;
    auto c2 = JsonSerializer::fromJson<MyGadget *>(json);
    QCOMPARE(c->intValue(), c2->intValue());
    QCOMPARE(c->stringValue(), c2->stringValue());
    QCOMPARE(c->doubleValue(), c2->doubleValue());

    delete c;
    delete c2;
}

void BasicTest::testAllVectorLikeContainers()
{
    using namespace JsonSerializer;

    // ---------- 1. Prepare source data ----------
    const QList<int> source = {10, 20, 30, 40, 50};

    // ---------- 2. Create all vector-like containers ----------
    QList<int> qlist;
    QVector<int> qvector;
    QStack<int> qstack;
    QQueue<int> qqueue;

    std::vector<int> stdvector;
    std::list<int> stdlist;
    std::deque<int> stddeque;
    std::array<int, 5> stdarray{};

    // ---------- 3. Fill all containers with the same data (one loop) ----------
    for (int value : source) {
        qlist << value;
        qvector << value;
        qstack.push(value);
        qqueue.enqueue(value);

        stdvector.push_back(value);
        stdlist.push_back(value);
        stddeque.push_back(value);
    }

    // std::array needs special handling (fixed size)
    for (int i = 0; i < source.size(); ++i)
        stdarray[i] = source[i];

    // ---------- 4. Helper lambda to test one container ----------
    auto testContainer = [](const auto &original, const char *name) {
        using Container = std::decay_t<decltype(original)>;

        // Serialize
        QJsonValue json = toJson(original);
        QVERIFY2(json.isArray(), qPrintable(QString("%1: expected JSON array").arg(name)));
        QCOMPARE(json.toArray().size(), static_cast<int>(original.size()));

        // Deserialize
        Container restored = fromJson<Container>(json);

        // Compare size
        QCOMPARE(static_cast<int>(restored.size()), static_cast<int>(original.size()));

        // Compare content
        auto it1 = original.begin();
        auto it2 = restored.begin();
        int index = 0;
        while (it1 != original.end()) {
            QCOMPARE(*it1, *it2);
            ++it1;
            ++it2;
            ++index;
        }

        qDebug() << "✓" << name << "passed";
    };

    // ---------- 5. Run tests for all containers ----------
    testContainer(qlist, "QList");
    testContainer(qvector, "QVector");
    testContainer(qstack, "QStack");
    testContainer(qqueue, "QQueue");

    testContainer(stdvector, "std::vector");
    testContainer(stdlist, "std::list");
    testContainer(stddeque, "std::deque");
    testContainer(stdarray, "std::array");
}

int MyGadget::intValue() const
{
    return m_intValue;
}

void MyGadget::setIntValue(int newIntValue)
{
    m_intValue = newIntValue;
}

QString MyGadget::stringValue() const
{
    return m_stringValue;
}

void MyGadget::setStringValue(const QString &newStringValue)
{
    m_stringValue = newStringValue;
}

double MyGadget::doubleValue() const
{
    return m_doubleValue;
}

void MyGadget::setDoubleValue(double newDoubleValue)
{
    m_doubleValue = newDoubleValue;
}

QTEST_MAIN(BasicTest)
#include "tst_basic.moc"
