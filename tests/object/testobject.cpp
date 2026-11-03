#include "testobject.h"


#include <QImage>
#include <QPixmap>
#include <QBitmap>
#include <QIcon>
#include <QRandomGenerator>
#include <QPainter>

// تولید QImage با پیکسل‌های تصادفی (RGBA)
QImage createRandomImage(int width, int height)
{
    QImage image(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(image.scanLine(y));
        for (int x = 0; x < width; ++x) {
            // تولید رنگ تصادفی با آلفا کامل
            quint8 r = QRandomGenerator::global()->bounded(256);
            quint8 g = QRandomGenerator::global()->bounded(256);
            quint8 b = QRandomGenerator::global()->bounded(256);
            line[x] = qRgba(r, g, b, 255);
        }
    }
    return image;
}

// تولید QPixmap با پیکسل‌های تصادفی
QPixmap createRandomPixmap(int width, int height)
{
    return QPixmap::fromImage(createRandomImage(width, height));
}

// تولید QBitmap (سیاه و سفید) با پیکسل‌های تصادفی
QBitmap createRandomBitmap(int width, int height)
{
    QImage image(width, height, QImage::Format_Mono);

    for (int y = 0; y < height; ++y) {
        uchar *line = image.scanLine(y);
        for (int x = 0; x < width; ++x) {
            // هر پیکسل با احتمال ۵۰٪ سیاه یا سفید می‌شود
            bool isBlack = QRandomGenerator::global()->bounded(2) == 0;
            if (isBlack)
                line[x / 8] |= (1 << (7 - (x % 8)));   // بیت را ۱ کن (سیاه)
            else
                line[x / 8] &= ~(1 << (7 - (x % 8)));  // بیت را ۰ کن (سفید)
        }
    }

    return QBitmap::fromImage(image);
}

// تولید QIcon با پیکسل‌های تصادفی
QIcon createRandomIcon(int width = 64, int height = 64)
{
    return QIcon(createRandomPixmap(width, height));
}

TestObject::TestObject(QObject *parent)
    : QObject(parent)
{}

void TestObject::fillRandom()
{
    auto *rng = QRandomGenerator::global();

    p_bool = rng->bounded(2) == 1;
    p_char = static_cast<char>(rng->bounded(32, 127));
    p_schar = static_cast<signed char>(rng->bounded(-128, 127));
    p_uchar = static_cast<uchar>(rng->bounded(0, 255));
    p_short = static_cast<short>(rng->bounded(-32768, 32767));
    p_ushort = static_cast<ushort>(rng->bounded(0, 65535));
    p_int = static_cast<int>(rng->bounded(-100000, 100000));
    p_uint = static_cast<uint>(rng->bounded(0, 200000));
    p_long = static_cast<long>(rng->bounded(-1000000, 1000000));
    p_ulong = static_cast<ulong>(rng->bounded(0, 2000000));
    p_qint64 = static_cast<qint64>(rng->generate64());
    p_quint64 = static_cast<quint64>(rng->generate64());
    p_float = rng->generateDouble() * 1000.0f;
    p_double = rng->generateDouble() * 10000.0;
    p_qfloat16 = qfloat16(rng->generateDouble() * 100.0);

    p_qchar = QChar(static_cast<char16_t>(rng->bounded(0x20, 0x7E)));
    p_string = QString("str_%1").arg(rng->bounded(10000));
    p_bytearray = QByteArray("bytes_") + QByteArray::number(rng->bounded(10000));
    p_date = QDate(2000 + rng->bounded(50), 1 + rng->bounded(12), 1 + rng->bounded(28));
    p_time = QTime(rng->bounded(24), rng->bounded(60), rng->bounded(60));
    p_datetime = QDateTime(p_date, p_time, QTimeZone::UTC);
    p_url = QUrl(QString("https://example.com/%1").arg(rng->bounded(1000)));
    p_uuid = QUuid::createUuid();
    p_color = QColor::fromRgb(rng->bounded(256), rng->bounded(256), rng->bounded(256));

    p_point = QPoint(rng->bounded(-500, 500), rng->bounded(-500, 500));
    p_pointf = QPointF(rng->generateDouble() * 100, rng->generateDouble() * 100);
    p_size = QSize(rng->bounded(1, 1000), rng->bounded(1, 1000));
    p_sizef = QSizeF(rng->generateDouble() * 500, rng->generateDouble() * 500);
    p_rect = QRect(p_point, p_size);
    p_rectf = QRectF(p_pointf, p_sizef);
    p_line = QLine(p_point, QPoint(p_point.x() + 50, p_point.y() + 50));
    p_linef = QLineF(p_pointf, QPointF(p_pointf.x() + 50, p_pointf.y() + 50));

    p_polygon = QPolygon({QPoint(0, 0), QPoint(10, 0), QPoint(5, 10)});
    p_polygonf = QPolygonF({QPointF(0, 0), QPointF(10.5, 0), QPointF(5.5, 10.5)});

    p_bitarray = QBitArray(16);
    for (int i = 0; i < 16; ++i)
        p_bitarray.setBit(i, rng->bounded(2));

    p_image = QImage(16, 16, QImage::Format_ARGB32);
    p_image.fill(p_color);
    p_pixmap = createRandomPixmap(24, 24);
    p_icon = QIcon(p_pixmap);
    p_bitmap = QBitmap::fromImage(p_image.createAlphaMask());
    p_region = QRegion(p_rect);

    p_font = QFont("Arial", 10 + rng->bounded(20));
    p_regexp = QRegularExpression(QString("pattern_%1").arg(rng->bounded(100)));
    p_bytearraylist = QByteArrayList{"one", "two", "three"};
    p_brush = QBrush(p_color);
    p_palette = QPalette(p_color);
    p_pen = QPen(p_color, 2.0);
    p_keysequence = QKeySequence(Qt::CTRL | Qt::Key_S);
    p_textformat = QTextFormat();
    p_transform = QTransform().rotate(rng->bounded(360));
    p_colorspace = QColorSpace(QColorSpace::SRgb);
    p_easingcurve = QEasingCurve(static_cast<QEasingCurve::Type>(rng->bounded(40)));
    p_locale = QLocale(QLocale::English, QLocale::UnitedStates);
    p_matrix4x4 = QMatrix4x4();
    p_matrix4x4.rotate(45, 0, 0, 1);

    // Containers
    p_qlist.clear();
    p_qvector.clear();
    p_qset.clear();
    p_qmap.clear();
    p_qhash.clear();
    p_qqueue.clear();
    p_qstack.clear();
    p_qlist_point.clear();

    for (int i = 0; i < 5; ++i) {
        p_qlist << rng->bounded(1000);
        p_qvector << QString("item_%1").arg(i);
        p_qset.insert(rng->bounded(100));
        p_qmap.insert(QString("key_%1").arg(i), rng->bounded(100));
        p_qhash.insert(QString("hkey_%1").arg(i), rng->generateDouble());
        p_qqueue.enqueue(rng->bounded(100));
        p_qstack.push(QString("stack_%1").arg(i));
        p_qlist_point << QPoint(i, i * 2);
    }
}

QPixmap TestObject::pixmap() const
{
    return m_pixmap;
}

void TestObject::setPixmap(const QPixmap &newPixmap)
{
    m_pixmap = newPixmap;
}

QIcon TestObject::icon() const
{
    return m_icon;
}

void TestObject::setIcon(const QIcon &newIcon)
{
    m_icon = newIcon;
}

QBitmap TestObject::bitmap() const
{
    return m_bitmap;
}

void TestObject::setBitmap(const QBitmap &newBitmap)
{
    m_bitmap = newBitmap;
}
