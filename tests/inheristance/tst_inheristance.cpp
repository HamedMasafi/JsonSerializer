#include "animalobject.h"
#include "lionobject.h"
#include "tigerobject.h"

#include <jsonserialize.h>

#include <QObject>
#include <QtTest>

class AnimalGadget
{
    Q_GADGET
    JSON_SERIALIZER_GADGET(AnimalGadget)
    JSON_SERIALIZER_BASE_GADGET
    Q_CLASSINFO("type", "derived")
};

class Tiger : public AnimalGadget
{
    Q_GADGET
    JSON_SERIALIZER_DERIVED_GADGET
    JSON_SERIALIZER_GADGET(Tiger)

};

class Lion : public AnimalGadget
{
    Q_GADGET
    JSON_SERIALIZER_DERIVED_GADGET
    JSON_SERIALIZER_GADGET(Lion)
    Q_CLASSINFO("type", "derived")
    // QT_ANNOTATE_CLASS(qt_qgadget, "")
};

class InheristanceTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testAnimalsGadgets();
    void testAnimalsObjects();
};

void InheristanceTest::initTestCase() {
    qDebug() << qRegisterMetaType<AnimalObject *>();
    qDebug() << qRegisterMetaType<TigerObject *>();
    qDebug() << qRegisterMetaType<LionObject *>();
}

void InheristanceTest::testAnimalsGadgets()
{
    AnimalGadget *a = new Lion;
    qDebug() << a->staticMetaObject.className() << a->staticMetaObject.superClass()
             << a->metaObject()->className() << a->metaObject()->classInfo(0).value();
    QList<AnimalGadget *> animals;

    animals << new AnimalGadget;
    animals << new Tiger;
    animals << new Lion;

    auto json = JsonSerializer::toJson(animals);
    qDebug() << "2" << json;
}

void InheristanceTest::testAnimalsObjects() {
    QList<AnimalObject *> animals;

    auto base = new AnimalObject;
    auto tiggy = new TigerObject;
    auto lio = new LionObject;

    base->setName("base");
    tiggy->setName("tiggy");
    lio->setName("lio");

    animals << base;
    animals << tiggy;
    animals << lio;
    auto json = JsonSerializer::toJson(animals);
    qDebug() << "2" << json;

    auto animals2 =JsonSerializer::fromJson<QList<AnimalObject*>>(json);

    qDebug() << animals2;
    QCOMPARE(animals.size(), animals2.size());

    auto tiggy2 = qobject_cast<TigerObject *>(animals2[1]);
    QVERIFY(tiggy2);

    auto leo2 = qobject_cast<LionObject *>(animals2[2]);
    QVERIFY(leo2);
}

QTEST_MAIN(InheristanceTest)
#include "tst_inheristance.moc"
