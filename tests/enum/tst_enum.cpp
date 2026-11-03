#include <jsonserialize.h>

#include <QObject>
#include <QtTest>

class MyGadget
{
    Q_GADGET

    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(MyFlags flag READ flag WRITE setFlag)

public:
    enum Type { User, Admin, Viewer };
    Q_ENUM(Type)

    enum MyFlag {
        Bit1 = 0x01,
        Bit2 = 0x02,
        Bit3 = 0x04,
        Bit4 = 0x08,
        Bit5 = 0x10,
        Bit6 = 0x20,
        Bit7 = 0x40,
        Bit8 = 0x80
    };
    Q_DECLARE_FLAGS(MyFlags, MyFlag)
    Q_FLAG(MyFlags)

    Type type() const;
    void setType(Type newType);

    MyFlags flag() const;
    void setFlag(const MyFlags &newFlag);

private:
    Type m_type;
    MyFlags m_flag;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MyGadget::MyFlags)

class EnumTest : public QObject
{
    Q_OBJECT

    enum MyEnum { One, Two, Three };
    Q_ENUM(MyEnum)

    enum SimpleEnum { A, B, C };
private slots:
    void testSimpleEnum_data();
    void testSimpleEnum();
    void testMetaEnum_data();
    void testMetaEnum();
    void testOnGadget_data();
    void testOnGadget();
};

void EnumTest::testSimpleEnum_data()
{
    QTest::addColumn<bool>("exportEnumAsString");

    QTest::newRow("enum as int") << false;
    QTest::newRow("enum as string") << true;
}

void EnumTest::testSimpleEnum()
{
    QFETCH(bool, exportEnumAsString);

    JsonSerializer::options.exportEnumAsString = exportEnumAsString;

    SimpleEnum en;
    en = B;
    auto json = JsonSerializer::toJson(en);

    if (exportEnumAsString) {
        QVERIFY(json.isDouble());
        QCOMPARE(int(json.toDouble()), 1);
    } else {
        QVERIFY(json.isDouble());
        QCOMPARE(int(json.toDouble()), 1);
    }

    qDebug() << json;
    auto en2 = JsonSerializer::fromJson<SimpleEnum>(json);
    QCOMPARE(en, en2);
}

void EnumTest::testMetaEnum_data()
{
    QTest::addColumn<bool>("exportEnumAsString");

    QTest::newRow("enum as int") << false;
    QTest::newRow("enum as string") << true;
}

void EnumTest::testMetaEnum()
{
    QFETCH(bool, exportEnumAsString);

    JsonSerializer::options.exportEnumAsString = exportEnumAsString;

    MyEnum en;
    en = Two;
    auto json = JsonSerializer::toJson(en);

    qDebug() << json;
    // QVERIFY(json.)
    if (exportEnumAsString) {
        QVERIFY(json.isString());
        QCOMPARE(json.toString(), "Two");
    } else {
        QVERIFY(json.isDouble());
        QCOMPARE(int(json.toDouble()), 1);
    }

    qDebug() << json;
    auto en2 = JsonSerializer::fromJson<MyEnum>(json);
    QCOMPARE(en, en2);
}

void EnumTest::testOnGadget_data()
{
    QTest::addColumn<bool>("exportEnumAsString");

    QTest::newRow("enum as int") << false;
    QTest::newRow("enum as string") << true;
}

void EnumTest::testOnGadget()
{
    QFETCH(bool, exportEnumAsString);

    JsonSerializer::options.exportEnumAsString = exportEnumAsString;

    auto g = new MyGadget;
    g->setType(MyGadget::Admin);
    g->setFlag(MyGadget::Bit2 | MyGadget::Bit5 | MyGadget::Bit7);
    auto json = JsonSerializer::toJson(g);
    qDebug() << json;
    auto g2 = JsonSerializer::fromJson<MyGadget *>(json);

    QVERIFY(g2);
    QCOMPARE(g->type(), g2->type());
    QCOMPARE(g->flag(), g2->flag());
}

MyGadget::Type MyGadget::type() const
{
    return m_type;
}

void MyGadget::setType(MyGadget::Type newType)
{
    m_type = newType;
}

MyGadget::MyFlags MyGadget::flag() const
{
    return m_flag;
}

void MyGadget::setFlag(const MyGadget::MyFlags &newFlag)
{
    m_flag = newFlag;
}

QTEST_MAIN(EnumTest)
#include "tst_enum.moc"
