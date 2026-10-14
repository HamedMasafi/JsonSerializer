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
#include <QMatrix4x4>
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

class VariantTest : public QObject
{
    Q_OBJECT

private slots:
    void testVariant_data();
    void testVariant();
};

void VariantTest::testVariant_data()
{
    QTest::addColumn<QVariant>("v");

    QTest::newRow("bool") << QVariant::fromValue(true);

    QTest::newRow("qint8") << QVariant::fromValue(qint8(-50));
    QTest::newRow("quint8") << QVariant::fromValue(quint8(100));
    QTest::newRow("qint16") << QVariant::fromValue(qint16(-1234));
    QTest::newRow("quint16") << QVariant::fromValue(quint16(1234));
    QTest::newRow("qint32") << QVariant::fromValue(qint32(-1234567));
    QTest::newRow("quint32") << QVariant::fromValue(quint32(1234567));
    QTest::newRow("qint64") << QVariant::fromValue(qint64(-1234567890123LL));
    QTest::newRow("quint64") << QVariant::fromValue(quint64(1234567890123ULL));

    QTest::newRow("char") << QVariant::fromValue('a');
    QTest::newRow("QChar") << QVariant::fromValue(QChar(u'Z'));

    QTest::newRow("float") << QVariant::fromValue(1.25f);
    QTest::newRow("double") << QVariant::fromValue(3.141592653589793);
    QTest::newRow("qfloat16") << QVariant::fromValue(qfloat16(1.5f));

    QTest::newRow("QString") << QVariant::fromValue(QString{"Hi"});
    QTest::newRow("QByteArray") << QVariant::fromValue(QByteArray{"Hello"});
    QTest::newRow("QUuid") << QVariant::fromValue(QUuid::createUuid());

    QTest::newRow("QTime") << QVariant::fromValue(QTime::currentTime());
    QTest::newRow("QDate") << QVariant::fromValue(QDate::currentDate());
    QTest::newRow("QDateTime") << QVariant::fromValue(QDateTime::currentDateTime());

    QTest::newRow("QUrl") << QVariant::fromValue(QUrl{"https://example.com"});
    QTest::newRow("QLocale") << QVariant::fromValue(QLocale{"en_US"});
    QTest::newRow("QRegularExpression") << QVariant::fromValue(QRegularExpression{"\\d+"});

    QTest::newRow("QPoint") << QVariant::fromValue(QPoint{10, 20});
    QTest::newRow("QPointF") << QVariant::fromValue(QPointF{1.5, 2.5});
    QTest::newRow("QSize") << QVariant::fromValue(QSize{640, 480});
    QTest::newRow("QSizeF") << QVariant::fromValue(QSizeF{640.5, 480.5});
    QTest::newRow("QRect") << QVariant::fromValue(QRect{0, 0, 100, 200});
    QTest::newRow("QRectF") << QVariant::fromValue(QRectF{0, 0, 100.5, 200.5});
    QTest::newRow("QLine") << QVariant::fromValue(QLine{0, 0, 10, 10});
    QTest::newRow("QLineF") << QVariant::fromValue(QLineF{0, 0, 10.5, 10.5});

    QTest::newRow("QColor") << QVariant::fromValue(QColor{Qt::red});
    QTest::newRow("QBrush") << QVariant::fromValue(QBrush{Qt::blue});
    QTest::newRow("QPen") << QVariant::fromValue(QPen{Qt::black});
    QTest::newRow("QFont") << QVariant::fromValue(QFont{"Arial", 12});
    QTest::newRow("QTransform") << QVariant::fromValue(QTransform{});
    QTest::newRow("QMatrix4x4") << QVariant::fromValue(QMatrix4x4{});

    QTest::newRow("QStringList") << QVariant::fromValue(QStringList{"one", "two"});
    QTest::newRow("QVariantList") << QVariant::fromValue(QVariantList{1, QString{"two"}, true});
    QTest::newRow("QVariantMap") << QVariant::fromValue(
        QVariantMap{{"number", 42}, {"text", QString{"Hello"}}});
    QTest::newRow("QVariantHash") << QVariant::fromValue(
        QVariantHash{{"number", 42}, {"text", QString{"Hello"}}});

    QTest::newRow("QByteArrayList") << QVariant::fromValue(QByteArrayList{"one", "two"});

    QTest::newRow("nullptr") << QVariant::fromValue(nullptr);
    QTest::newRow("invalid") << QVariant{};
}

void VariantTest::testVariant()
{
    QFETCH(QVariant, v);

    auto json = JsonSerializer::toJson(v);
    auto v2 = JsonSerializer::fromJson<QVariant>(json);
    QCOMPARE(v.type(), v2.type());
    QCOMPARE(v, v2);
}

QTEST_MAIN(VariantTest)
#include "tst_variant.moc"
