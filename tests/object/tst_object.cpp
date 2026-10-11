#include <QtTest>
#include "mygadget.h"
#include "myobject.h"

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
};

void ObjectTest::initTestCase()
{
    qDebug() << qRegisterMetaType<MyObject *>();
    qDebug() << qRegisterMetaType<MyGadget *>();
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

void ObjectTest::testWithChildGadget() {
    auto myObject = new MyObject;
    auto myGadget = new MyGadget;
    myGadget->setId(4567);
    myObject->setGadget(myGadget);

    auto json = JsonSerializer::toJson(myObject);
    auto myObject2 = JsonSerializer::fromJson<MyObject *>(json);
    QVERIFY(myObject2->gadget());
}

void ObjectTest::testList() {
    QList<MyGadget*> gadgets;
    for (auto i = 0; i < 20; ++i) {
        auto g = new MyGadget;
        g->setId(i);
        gadgets<<g;
    }

    auto json  = JsonSerializer::toJson(gadgets);
    auto list = JsonSerializer::fromJson<QList<MyGadget *>>(json);
    QCOMPARE(gadgets.size(), list.size());

    for (auto i = 0; i < 20; ++i)
        QCOMPARE(gadgets[i]->id(), list[i]->id());
}

void ObjectTest::testSharedPointerList() {
    using Type = QList<QSharedPointer<MyGadget>>;

Type gadgets;
    for (auto i = 0; i < 20; ++i) {
        auto g = QSharedPointer<MyGadget>{new MyGadget};
        g->setId(i);
        gadgets<<g;
    }

    auto json  = JsonSerializer::toJson(gadgets);
    auto list = JsonSerializer::fromJson<Type>(json);
    QCOMPARE(gadgets.size(), list.size());

    for (auto i = 0; i < 20; ++i)
        QCOMPARE(gadgets[i]->id(), list[i]->id());
}

QTEST_APPLESS_MAIN(ObjectTest)
#include "tst_object.moc"
