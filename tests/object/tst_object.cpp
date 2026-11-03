#include <QtTest>
#include "mygadget.h"
#include "myobject.h"
#include "testobject.h"

#include <jsonserialize.h>

class ObjectTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void simpleTest();
    void createObject();
    void createGadget();
    void testWithChildGadget();
    void testList();
    void testSharedPointerList();
    void testObjectWithProperties();
};

void ObjectTest::initTestCase()
{
    qDebug() << qRegisterMetaType<MyObject *>();
    qDebug() << qRegisterMetaType<MyGadget *>();
    qDebug() << qRegisterMetaType<QList<int>>();
    qDebug() << qRegisterMetaType<QSet<int>>();

    auto mt = QMetaType::fromName("QList<int>"); // don't get meta type from anywhere else; because this name will be read from disk
    // auto data = mt.create();
    QVariant var = QVariant::fromMetaType(mt); // یا QVariant(mt);

    auto it = var.view<QMetaSequence::Iterable>();

    it.append(QVariant::fromValue(1)); // Runtime crash here. The appand method is exists but it's not written in docs
}

void ObjectTest::simpleTest()
{
    auto myObject = new MyObject;

    myObject->setId(1234);
    myObject->setName("Json Serializer");
    myObject->setBackgroundColor(Qt::darkCyan);

    auto json = JsonSerializer::toJson(myObject);
    auto myObject2 = JsonSerializer::fromJson<MyObject *>(json);
    QCOMPARE(myObject->id(), myObject2->id());
    QCOMPARE(myObject->name(), myObject2->name());
    QCOMPARE(myObject->backgroundColor(), myObject2->backgroundColor());
}

void ObjectTest::createObject()
{
    auto o = JsonSerializer::Impl::createQObject("MyObject*");
    QVERIFY(o);
}

void ObjectTest::createGadget()
{
    auto o = JsonSerializer::Impl::createGadget("MyGadget*");
    QVERIFY(o.isValid());
}

void ObjectTest::testWithChildGadget()
{
    auto myObject = new MyObject;
    auto myGadget = new MyGadget;
    myGadget->setId(4567);
    myObject->setGadget(myGadget);

    auto json = JsonSerializer::toJson(myObject);
    auto myObject2 = JsonSerializer::fromJson<MyObject *>(json);
    QVERIFY(myObject2->gadget());
}

void ObjectTest::testList()
{
    QList<MyGadget *> gadgets;
    for (auto i = 0; i < 20; ++i) {
        auto g = new MyGadget;
        g->setId(i);
        gadgets << g;
    }

    auto json = JsonSerializer::toJson(gadgets);
    auto list = JsonSerializer::fromJson<QList<MyGadget *>>(json);
    QCOMPARE(gadgets.size(), list.size());

    for (auto i = 0; i < 20; ++i)
        QCOMPARE(gadgets[i]->id(), list[i]->id());
}

void ObjectTest::testSharedPointerList()
{
    using Type = QList<QSharedPointer<MyGadget>>;

    Type gadgets;
    for (auto i = 0; i < 20; ++i) {
        auto g = QSharedPointer<MyGadget>{new MyGadget};
        g->setId(i);
        gadgets << g;
    }

    auto json = JsonSerializer::toJson(gadgets);
    auto list = JsonSerializer::fromJson<Type>(json);
    QCOMPARE(gadgets.size(), list.size());

    for (auto i = 0; i < 20; ++i)
        QCOMPARE(gadgets[i]->id(), list[i]->id());
}

void ObjectTest::testObjectWithProperties()
{
    auto obj = new TestObject;
    obj->fillRandom();
    auto json = JsonSerializer::toJson(obj);
    auto obj2 = JsonSerializer::fromJson<TestObject *>(json);

    qDebug().noquote().nospace() << QJsonDocument(json.toObject()).toJson(QJsonDocument::Compact);
    QVERIFY(obj2);

    QCOMPARE(obj->p_bool, obj2->p_bool);
    QCOMPARE(obj->p_char, obj2->p_char);
    QCOMPARE(obj->p_schar, obj2->p_schar);
    QCOMPARE(obj->p_uchar, obj2->p_uchar);
    QCOMPARE(obj->p_short, obj2->p_short);
    QCOMPARE(obj->p_ushort, obj2->p_ushort);
    QCOMPARE(obj->p_int, obj2->p_int);
    QCOMPARE(obj->p_uint, obj2->p_uint);
    QCOMPARE(obj->p_long, obj2->p_long);
    QCOMPARE(obj->p_ulong, obj2->p_ulong);
    QCOMPARE(obj->p_qint64, obj2->p_qint64);
    QCOMPARE(obj->p_quint64, obj2->p_quint64);
    QCOMPARE(obj->p_float, obj2->p_float);
    QCOMPARE(obj->p_double, obj2->p_double);
    QCOMPARE(obj->p_qfloat16, obj2->p_qfloat16);

    QCOMPARE(obj->p_qchar, obj2->p_qchar);
    QCOMPARE(obj->p_string, obj2->p_string);
    QCOMPARE(obj->p_bytearray, obj2->p_bytearray);
    QCOMPARE(obj->p_date, obj2->p_date);
    QCOMPARE(obj->p_time, obj2->p_time);
    QCOMPARE(obj->p_datetime, obj2->p_datetime);
    QCOMPARE(obj->p_url, obj2->p_url);
    QCOMPARE(obj->p_uuid, obj2->p_uuid);
    QCOMPARE(obj->p_color, obj2->p_color);
    QCOMPARE(obj->p_point, obj2->p_point);
    QCOMPARE(obj->p_pointf, obj2->p_pointf);
    QCOMPARE(obj->p_size, obj2->p_size);
    QCOMPARE(obj->p_sizef, obj2->p_sizef);
    QCOMPARE(obj->p_rect, obj2->p_rect);
    QCOMPARE(obj->p_rectf, obj2->p_rectf);
    QCOMPARE(obj->p_line, obj2->p_line);
    QCOMPARE(obj->p_linef, obj2->p_linef);
    QCOMPARE(obj->p_polygon, obj2->p_polygon);
    QCOMPARE(obj->p_polygonf, obj2->p_polygonf);
    QCOMPARE(obj->p_bitarray, obj2->p_bitarray);

    // Image / Pixmap / Icon comparison (size + format is usually enough for tests)
    QCOMPARE(obj->p_image.size(), obj2->p_image.size());
    QCOMPARE(obj->p_image.format(), obj2->p_image.format());
    // QCOMPARE(obj->p_pixmap.size(), obj2->p_pixmap.size());
    // QCOMPARE(obj->p_bitmap.size(), obj2->p_bitmap.size());
    QCOMPARE(obj->p_region, obj2->p_region);

    QCOMPARE(obj->p_font, obj2->p_font);
    QCOMPARE(obj->p_regexp, obj2->p_regexp);
    QCOMPARE(obj->p_bytearraylist, obj2->p_bytearraylist);
    QCOMPARE(obj->p_brush, obj2->p_brush);
    QCOMPARE(obj->p_pen, obj2->p_pen);
    QCOMPARE(obj->p_keysequence, obj2->p_keysequence);
    QCOMPARE(obj->p_transform, obj2->p_transform);
    QCOMPARE(obj->p_easingcurve, obj2->p_easingcurve);
    QCOMPARE(obj->p_locale, obj2->p_locale);
    QCOMPARE(obj->p_matrix4x4, obj2->p_matrix4x4);

    // Containers
    QCOMPARE(obj->p_qlist, obj2->p_qlist);
    QCOMPARE(obj->p_qvector, obj2->p_qvector);
    QCOMPARE(obj->p_qset, obj2->p_qset);
    QCOMPARE(obj->p_qmap, obj2->p_qmap);
    QCOMPARE(obj->p_qhash, obj2->p_qhash);
    QCOMPARE(obj->p_qqueue, obj2->p_qqueue);
    QCOMPARE(obj->p_qstack, obj2->p_qstack);
    QCOMPARE(obj->p_qlist_point, obj2->p_qlist_point);
}

QTEST_MAIN(ObjectTest)
#include "tst_object.moc"
