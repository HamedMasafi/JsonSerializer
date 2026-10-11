#include "qttypes.h"

#include <QBuffer>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaEnum>

#include "jsonserialize.h"

namespace JsonSerializer::Impl {

template<>
QJsonValue basicTypeToJson<bool>(const bool &v)
{
    return QJsonValue{v};
}

template<>
bool jsonToBasicType<bool>(const QJsonValue &v)
{
    return v.toBool();
}

template<>
QJsonValue basicTypeToJson<int>(const int &v)
{
    return QJsonValue{v};
}
template<>
int jsonToBasicType<int>(const QJsonValue &v)
{
    return v.toInt();
}

template<>
QJsonValue basicTypeToJson<double>(const double &v)
{
    return QJsonValue{v};
}
template<>
double jsonToBasicType<double>(const QJsonValue &v)
{
    return v.toDouble();
}

template<>
QJsonValue basicTypeToJson<uint>(const uint &v)
{
    return QJsonValue(static_cast<qint64>(v));
}
template<>
uint jsonToBasicType<uint>(const QJsonValue &v)
{
    return v.toVariant().toUInt();
}

template<>
QJsonValue basicTypeToJson<qint64>(const qint64 &v)
{
    return QJsonValue(QString::number(v));
}
template<>
qint64 jsonToBasicType<qint64>(const QJsonValue &v)
{
    return v.toString().toLongLong();
}

template<>
QJsonValue basicTypeToJson<quint64>(const quint64 &v)
{
    return QJsonValue(QString::number(v));
}
template<>
quint64 jsonToBasicType<quint64>(const QJsonValue &v)
{
    return v.toString().toULongLong();
}

template<>
QJsonValue basicTypeToJson<long>(const long &v)
{
    return QJsonValue(QString::number(v));
}
template<>
long jsonToBasicType<long>(const QJsonValue &v)
{
    return v.toString().toLong();
}

template<>
QJsonValue basicTypeToJson<short>(const short &v)
{
    return QJsonValue(v);
}
template<>
short jsonToBasicType<short>(const QJsonValue &v)
{
    return static_cast<short>(v.toInt());
}

template<>
QJsonValue basicTypeToJson<char>(const char &v)
{
    return QJsonValue(v);
}
template<>
char jsonToBasicType<char>(const QJsonValue &v)
{
    return static_cast<char>(v.toInt());
}

template<>
QJsonValue basicTypeToJson<ulong>(const ulong &v)
{
    return QJsonValue(QString::number(v));
}
template<>
ulong jsonToBasicType<ulong>(const QJsonValue &v)
{
    return v.toString().toULong();
}

template<>
QJsonValue basicTypeToJson<ushort>(const ushort &v)
{
    return QJsonValue(v);
}
template<>
ushort jsonToBasicType<ushort>(const QJsonValue &v)
{
    return static_cast<ushort>(v.toInt());
}

template<>
QJsonValue basicTypeToJson<uchar>(const uchar &v)
{
    return QJsonValue(v);
}
template<>
uchar jsonToBasicType<uchar>(const QJsonValue &v)
{
    return static_cast<uchar>(v.toInt());
}

template<>
QJsonValue basicTypeToJson<float>(const float &v)
{
    return QJsonValue(static_cast<double>(v));
}
template<>
float jsonToBasicType<float>(const QJsonValue &v)
{
    return static_cast<float>(v.toDouble());
}

template<>
QJsonValue basicTypeToJson<signed char>(const signed char &v)
{
    return QJsonValue(v);
}
template<>
signed char jsonToBasicType<signed char>(const QJsonValue &v)
{
    return static_cast<signed char>(v.toInt());
}

// QString - رشته ساده
template<>
QJsonValue basicTypeToJson<QString>(const QString &v)
{
    return QJsonValue(v);
}
template<>
QString jsonToBasicType<QString>(const QJsonValue &v)
{
    return v.toString();
}

// QChar
template<>
QJsonValue basicTypeToJson<QChar>(const QChar &v)
{
    return QJsonValue(QString(v));
}
template<>
QChar jsonToBasicType<QChar>(const QJsonValue &v)
{
    return v.toString().at(0);
}

// QByteArray
template<>
QJsonValue basicTypeToJson<QByteArray>(const QByteArray &v)
{
    return QJsonValue(QString::fromLatin1(v.toBase64()));
}
template<>
QByteArray jsonToBasicType<QByteArray>(const QJsonValue &v)
{
    return QByteArray::fromBase64(v.toString().toLatin1());
}

// QDate
template<>
QJsonValue basicTypeToJson<QDate>(const QDate &v)
{
    return QJsonValue(v.toString(Qt::ISODate));
}
template<>
QDate jsonToBasicType<QDate>(const QJsonValue &v)
{
    return QDate::fromString(v.toString(), Qt::ISODate);
}

// QTime
template<>
QJsonValue basicTypeToJson<QTime>(const QTime &v)
{
    return QJsonValue(v.toString(Qt::ISODateWithMs));
}
template<>
QTime jsonToBasicType<QTime>(const QJsonValue &v)
{
    return QTime::fromString(v.toString(), Qt::ISODateWithMs);
}

// QDateTime
template<>
QJsonValue basicTypeToJson<QDateTime>(const QDateTime &v)
{
    return QJsonValue(v.toString(Qt::ISODateWithMs));
}
template<>
QDateTime jsonToBasicType<QDateTime>(const QJsonValue &v)
{
    return QDateTime::fromString(v.toString(), Qt::ISODateWithMs);
}

// QUrl
template<>
QJsonValue basicTypeToJson<QUrl>(const QUrl &v)
{
    return QJsonValue(v.toString());
}
template<>
QUrl jsonToBasicType<QUrl>(const QJsonValue &v)
{
    return QUrl(v.toString());
}

// QPoint
template<>
QJsonValue basicTypeToJson<QPoint>(const QPoint &v)
{
    QJsonObject valueObject;
    valueObject["x"] = v.x();
    valueObject["y"] = v.y();
    return valueObject;
}
template<>
QPoint jsonToBasicType<QPoint>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QPoint{valueObject["x"].toInt(), valueObject["y"].toInt()};
}

// QPointF
template<>
QJsonValue basicTypeToJson<QPointF>(const QPointF &v)
{
    QJsonObject valueObject;
    valueObject["x"] = v.x();
    valueObject["y"] = v.y();
    return valueObject;
}
template<>
QPointF jsonToBasicType<QPointF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QPointF{valueObject["x"].toDouble(), valueObject["y"].toDouble()};
}

// QSize
template<>
QJsonValue basicTypeToJson<QSize>(const QSize &v)
{
    QJsonObject valueObject;
    valueObject["w"] = v.width();
    valueObject["h"] = v.height();
    return valueObject;
}
template<>
QSize jsonToBasicType<QSize>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QSize{valueObject["w"].toInt(), valueObject["h"].toInt()};
}

// QSizeF
template<>
QJsonValue basicTypeToJson<QSizeF>(const QSizeF &v)
{
    QJsonObject valueObject;
    valueObject["w"] = v.width();
    valueObject["h"] = v.height();
    return valueObject;
}
template<>
QSizeF jsonToBasicType<QSizeF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QSizeF{valueObject["w"].toDouble(), valueObject["h"].toDouble()};
}

// QRect
template<>
QJsonValue basicTypeToJson<QRect>(const QRect &v)
{
    QJsonObject valueObject;
    valueObject["x"] = v.x();
    valueObject["y"] = v.y();
    valueObject["w"] = v.width();
    valueObject["h"] = v.height();
    return valueObject;
}
template<>
QRect jsonToBasicType<QRect>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QRect{valueObject["x"].toInt(),
                 valueObject["y"].toInt(),
                 valueObject["w"].toInt(),
                 valueObject["h"].toInt()};
}

// QRectF
template<>
QJsonValue basicTypeToJson<QRectF>(const QRectF &v)
{
    QJsonObject valueObject;
    valueObject["x"] = v.x();
    valueObject["y"] = v.y();
    valueObject["w"] = v.width();
    valueObject["h"] = v.height();
    return valueObject;
}
template<>
QRectF jsonToBasicType<QRectF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QRectF{valueObject["x"].toDouble(),
                  valueObject["y"].toDouble(),
                  valueObject["w"].toDouble(),
                  valueObject["h"].toDouble()};
}

// QLine
template<>
QJsonValue basicTypeToJson<QLine>(const QLine &v)
{
    QJsonObject valueObject;
    valueObject["x1"] = v.x1();
    valueObject["y1"] = v.y1();
    valueObject["x2"] = v.x2();
    valueObject["y2"] = v.y2();
    return valueObject;
}
template<>
QLine jsonToBasicType<QLine>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QLine{valueObject["x1"].toInt(),
                 valueObject["y1"].toInt(),
                 valueObject["x2"].toInt(),
                 valueObject["y2"].toInt()};
}

// QLineF
template<>
QJsonValue basicTypeToJson<QLineF>(const QLineF &v)
{
    QJsonObject valueObject;
    valueObject["x1"] = v.x1();
    valueObject["y1"] = v.y1();
    valueObject["x2"] = v.x2();
    valueObject["y2"] = v.y2();
    return valueObject;
}
template<>
QLineF jsonToBasicType<QLineF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QLineF{valueObject["x1"].toDouble(),
                  valueObject["y1"].toDouble(),
                  valueObject["x2"].toDouble(),
                  valueObject["y2"].toDouble()};
}

// QUuid
template<>
QJsonValue basicTypeToJson<QUuid>(const QUuid &v)
{
    return QJsonValue(v.toString(QUuid::WithoutBraces));
}
template<>
QUuid jsonToBasicType<QUuid>(const QJsonValue &v)
{
    return QUuid{v.toString()};
}

// QColor
template<>
QJsonValue basicTypeToJson<QColor>(const QColor &v)
{
    QJsonObject valueObject;
    valueObject["red"] = v.red();
    valueObject["green"] = v.green();
    valueObject["blue"] = v.blue();
    valueObject["alpha"] = v.alpha();
    return valueObject;
}
template<>
QColor jsonToBasicType<QColor>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QColor(valueObject["red"].toInt(),
                  valueObject["green"].toInt(),
                  valueObject["blue"].toInt(),
                  valueObject["alpha"].toInt(255));
}

// QPolygon
template<>
QJsonValue basicTypeToJson<QPolygon>(const QPolygon &v)
{
    QJsonArray pointsArray;
    for (const QPoint &p : v) {
        QJsonObject pointObject;
        pointObject["x"] = p.x();
        pointObject["y"] = p.y();
        pointsArray.append(pointObject);
    }
    QJsonObject valueObject;
    valueObject["points"] = pointsArray;
    return valueObject;
}
template<>
QPolygon jsonToBasicType<QPolygon>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto pointsArray = valueObject["points"].toArray();
    QPolygon poly;
    for (const auto &p : std::as_const(pointsArray)) {
        auto pointObject = p.toObject();
        poly << QPoint{pointObject["x"].toInt(), pointObject["y"].toInt()};
    }
    return poly;
}

// QPolygonF
template<>
QJsonValue basicTypeToJson<QPolygonF>(const QPolygonF &v)
{
    QJsonArray pointsArray;
    for (const QPointF &p : v) {
        QJsonObject pointObject;
        pointObject["x"] = p.x();
        pointObject["y"] = p.y();
        pointsArray.append(pointObject);
    }
    QJsonObject valueObject;
    valueObject["points"] = pointsArray;
    return valueObject;
}
template<>
QPolygonF jsonToBasicType<QPolygonF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto pointsArray = valueObject["points"].toArray();
    QPolygonF poly;
    for (const auto &p : std::as_const(pointsArray)) {
        auto pointObject = p.toObject();
        poly << QPointF{pointObject["x"].toDouble(), pointObject["y"].toDouble()};
    }
    return poly;
}

// QBitArray
template<>
QJsonValue basicTypeToJson<QBitArray>(const QBitArray &v)
{
    QString bits;
    bits.reserve(v.size());
    for (int i = 0; i < v.size(); ++i)
        bits.append(v.testBit(i) ? '1' : '0');
    QJsonObject valueObject;
    valueObject["bits"] = bits;
    return valueObject;
}
template<>
QBitArray jsonToBasicType<QBitArray>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto bits = valueObject["bits"].toString();
    QBitArray bitArray(bits.size());
    for (int i = 0; i < bits.size(); ++i)
        bitArray.setBit(i, bits[i] == '1');
    return bitArray;
}

// QImage
template<>
QJsonValue basicTypeToJson<QImage>(const QImage &v)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    v.save(&buffer, "PNG");
    return QJsonValue(QString::fromLatin1(bytes.toBase64()));
}
template<>
QImage jsonToBasicType<QImage>(const QJsonValue &v)
{
    auto bytes = QByteArray::fromBase64(v.toString().toLatin1());
    QImage image;
    image.loadFromData(bytes, "PNG");
    return image;
}

// QPixmap
template<>
QJsonValue basicTypeToJson<QPixmap>(const QPixmap &v)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    v.save(&buffer, "PNG");
    return QJsonValue(QString::fromLatin1(bytes.toBase64()));
}
template<>
QPixmap jsonToBasicType<QPixmap>(const QJsonValue &v)
{
    auto bytes = QByteArray::fromBase64(v.toString().toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(bytes, "PNG");
    return pixmap;
}

// QIcon
template<>
QJsonValue basicTypeToJson<QIcon>(const QIcon &v)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    auto pixmap = v.pixmap(256, 256);
    pixmap.save(&buffer, "PNG");
    return QJsonValue(QString::fromLatin1(bytes.toBase64()));
}
template<>
QIcon jsonToBasicType<QIcon>(const QJsonValue &v)
{
    auto bytes = QByteArray::fromBase64(v.toString().toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(bytes, "PNG");
    return QIcon{pixmap};
}

// QRegion
template<>
QJsonValue basicTypeToJson<QRegion>(const QRegion &v)
{
    QJsonArray rectsArray;
    for (const QRect &r : v) {
        QJsonObject rectObject;
        rectObject["x"] = r.x();
        rectObject["y"] = r.y();
        rectObject["w"] = r.width();
        rectObject["h"] = r.height();
        rectsArray.append(rectObject);
    }
    QJsonObject valueObject;
    valueObject["rects"] = rectsArray;
    return valueObject;
}
template<>
QRegion jsonToBasicType<QRegion>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto rectsArray = valueObject["rects"].toArray();
    QRegion region;
    for (const auto &r : std::as_const(rectsArray)) {
        auto rectObject = r.toObject();
        region += QRect{rectObject["x"].toInt(),
                        rectObject["y"].toInt(),
                        rectObject["w"].toInt(),
                        rectObject["h"].toInt()};
    }
    return region;
}

// QFont
template<>
QJsonValue basicTypeToJson<QFont>(const QFont &v)
{
    QJsonObject valueObject;
    valueObject["family"] = v.family();
    valueObject["pointSizeF"] = v.pointSizeF();
    valueObject["weight"] = v.weight();
    valueObject["italic"] = v.italic();
    valueObject["underline"] = v.underline();
    valueObject["strikeOut"] = v.strikeOut();
    valueObject["kerning"] = v.kerning();
    valueObject["letterSpacing"] = v.letterSpacing();
    valueObject["wordSpacing"] = v.wordSpacing();
    valueObject["capitalization"] = static_cast<int>(v.capitalization());
    return valueObject;
}
template<>
QFont jsonToBasicType<QFont>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    QFont font;
    font.setFamily(valueObject["family"].toString());
    font.setPointSizeF(valueObject["pointSizeF"].toDouble());
    font.setWeight(static_cast<QFont::Weight>(valueObject["weight"].toInt()));
    font.setItalic(valueObject["italic"].toBool());
    font.setUnderline(valueObject["underline"].toBool());
    font.setStrikeOut(valueObject["strikeOut"].toBool());
    font.setKerning(valueObject["kerning"].toBool());
    font.setLetterSpacing(QFont::PercentageSpacing, valueObject["letterSpacing"].toDouble());
    font.setWordSpacing(valueObject["wordSpacing"].toDouble());
    font.setCapitalization(
        static_cast<QFont::Capitalization>(valueObject["capitalization"].toInt()));
    return font;
}

// QRegularExpression
template<>
QJsonValue basicTypeToJson<QRegularExpression>(const QRegularExpression &v)
{
    QJsonObject valueObject;
    valueObject["pattern"] = v.pattern();
    valueObject["options"] = static_cast<int>(v.patternOptions());
    return valueObject;
}
template<>
QRegularExpression jsonToBasicType<QRegularExpression>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QRegularExpression{valueObject["pattern"].toString(),
                              static_cast<QRegularExpression::PatternOptions>(
                                  valueObject["options"].toInt())};
}

// QByteArrayList
template<>
QJsonValue basicTypeToJson<QByteArrayList>(const QByteArrayList &v)
{
    QJsonArray array;
    for (const auto &ba : v)
        array.append(QString::fromLatin1(ba.toBase64()));
    QJsonObject valueObject;
    valueObject["list"] = array;
    return valueObject;
}
template<>
QByteArrayList jsonToBasicType<QByteArrayList>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto array = valueObject["list"].toArray();
    QByteArrayList list;
    for (const auto &item : std::as_const(array))
        list.append(QByteArray::fromBase64(item.toString().toLatin1()));
    return list;
}

// QBrush
template<>
QJsonValue basicTypeToJson<QBrush>(const QBrush &v)
{
    QJsonObject colorObject;
    colorObject["r"] = v.color().red();
    colorObject["g"] = v.color().green();
    colorObject["b"] = v.color().blue();
    colorObject["a"] = v.color().alpha();
    QJsonObject valueObject;
    valueObject["style"] = static_cast<int>(v.style());
    valueObject["color"] = colorObject;
    return valueObject;
}
template<>
QBrush jsonToBasicType<QBrush>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto colorObject = valueObject["color"].toObject();
    QColor color{colorObject["r"].toInt(),
                 colorObject["g"].toInt(),
                 colorObject["b"].toInt(),
                 colorObject["a"].toInt()};
    return QBrush{color, static_cast<Qt::BrushStyle>(valueObject["style"].toInt())};
}

// QPalette
template<>
QJsonValue basicTypeToJson<QPalette>(const QPalette &v)
{
    QJsonObject valueObject;

    QList<QPalette::ColorRole> roles{QPalette::ColorRole::WindowText,
                                     QPalette::ColorRole::Button,
                                     QPalette::ColorRole::Light,
                                     QPalette::ColorRole::Midlight,
                                     QPalette::ColorRole::Dark,
                                     QPalette::ColorRole::Mid,
                                     QPalette::ColorRole::Text,
                                     QPalette::ColorRole::BrightText,
                                     QPalette::ColorRole::ButtonText,
                                     QPalette::ColorRole::Base,
                                     QPalette::ColorRole::Window,
                                     QPalette::ColorRole::Shadow,
                                     QPalette::ColorRole::Highlight,
                                     QPalette::ColorRole::HighlightedText,
                                     QPalette::ColorRole::Link,
                                     QPalette::ColorRole::LinkVisited,
                                     QPalette::ColorRole::AlternateBase,
                                     QPalette::ColorRole::NoRole,
                                     QPalette::ColorRole::ToolTipBase,
                                     QPalette::ColorRole::ToolTipText,
                                     QPalette::ColorRole::PlaceholderText,
                                     QPalette::ColorRole::Accent};
    auto colorRoleEnum = QMetaEnum::fromType<QPalette::ColorRole>();

    for (auto r : roles) {
        auto name = QString::fromUtf8(colorRoleEnum.valueToKey(r));

        QJsonObject groupObject;
        auto colorActive = v.color(QPalette::ColorGroup::Active, r);
        auto colorDisabled = v.color(QPalette::ColorGroup::Disabled, r);
        auto colorInactive = v.color(QPalette::ColorGroup::Inactive, r);
        groupObject["active"] = toJson(colorActive);
        groupObject["disabled"] = toJson(colorDisabled);
        groupObject["inactive"] = toJson(colorInactive);
        valueObject[name] = groupObject;
    }
    // TODO: save QPalette::brush
    return valueObject;
}
template<>
QPalette jsonToBasicType<QPalette>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    QList<QPalette::ColorRole> roles{QPalette::ColorRole::WindowText,
                                     QPalette::ColorRole::Button,
                                     QPalette::ColorRole::Light,
                                     QPalette::ColorRole::Midlight,
                                     QPalette::ColorRole::Dark,
                                     QPalette::ColorRole::Mid,
                                     QPalette::ColorRole::Text,
                                     QPalette::ColorRole::BrightText,
                                     QPalette::ColorRole::ButtonText,
                                     QPalette::ColorRole::Base,
                                     QPalette::ColorRole::Window,
                                     QPalette::ColorRole::Shadow,
                                     QPalette::ColorRole::Highlight,
                                     QPalette::ColorRole::HighlightedText,
                                     QPalette::ColorRole::Link,
                                     QPalette::ColorRole::LinkVisited,
                                     QPalette::ColorRole::AlternateBase,
                                     QPalette::ColorRole::NoRole,
                                     QPalette::ColorRole::ToolTipBase,
                                     QPalette::ColorRole::ToolTipText,
                                     QPalette::ColorRole::PlaceholderText,
                                     QPalette::ColorRole::Accent};
    QPalette p;

    auto colorRoleEnum = QMetaEnum::fromType<QPalette::ColorRole>();

    for (auto r : roles) {
        auto name = QString::fromUtf8(colorRoleEnum.valueToKey(r));

        if (!valueObject[name].isObject())
            continue;
        auto o = valueObject[name].toObject();

        auto colorActive = fromJson<QColor>(o["active"]);
        auto colorDisabled = fromJson<QColor>(o["disabled"]);
        auto colorInactive = fromJson<QColor>(o["inactive"]);
        p.setColor(QPalette::ColorGroup::Active, r, colorActive);
        p.setColor(QPalette::ColorGroup::Disabled, r, colorDisabled);
        p.setColor(QPalette::ColorGroup::Inactive, r, colorInactive);
    }

    return p;
}

// QPen
template<>
QJsonValue basicTypeToJson<QPen>(const QPen &v)
{
    QJsonObject colorObject;
    colorObject["r"] = v.color().red();
    colorObject["g"] = v.color().green();
    colorObject["b"] = v.color().blue();
    colorObject["a"] = v.color().alpha();

    QJsonObject valueObject;

    valueObject["widthF"] = v.widthF();
    valueObject["style"] = static_cast<int>(v.style());
    valueObject["capStyle"] = static_cast<int>(v.capStyle());
    valueObject["joinStyle"] = static_cast<int>(v.joinStyle());
    valueObject["color"] = colorObject;

    return valueObject;
}

template<>
QPen jsonToBasicType<QPen>(const QJsonValue &j)
{
    auto obj = j.toObject();
    auto colorObject = obj["color"].toObject();

    QColor color{colorObject["r"].toInt(),
                 colorObject["g"].toInt(),
                 colorObject["b"].toInt(),
                 colorObject["a"].toInt()};

    QPen pen;

    pen.setWidthF(obj["widthF"].toDouble());
    pen.setStyle(static_cast<Qt::PenStyle>(obj["style"].toInt()));
    pen.setCapStyle(static_cast<Qt::PenCapStyle>(obj["capStyle"].toInt()));
    pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(obj["joinStyle"].toInt()));
    pen.setColor(color);

    return pen;
}
// QBitmap
template<>
QJsonValue basicTypeToJson<QBitmap>(const QBitmap &v)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);

    v.save(&buffer, "PNG");

    return QJsonValue{QString::fromLatin1(bytes.toBase64())};
}

template<>
QBitmap jsonToBasicType<QBitmap>(const QJsonValue &j)
{
    auto bytes = QByteArray::fromBase64(j.toString().toLatin1());

    QBitmap bitmap;
    bitmap.loadFromData(bytes, "PNG");

    return bitmap;
}

// QKeySequence
template<>
QJsonValue basicTypeToJson<QKeySequence>(const QKeySequence &v)
{
    return QJsonValue{v.toString(QKeySequence::PortableText)};
}

template<>
QKeySequence jsonToBasicType<QKeySequence>(const QJsonValue &j)
{
    return QKeySequence{j.toString(), QKeySequence::PortableText};
}

// QTextFormat
template<>
QJsonValue basicTypeToJson<QTextFormat>(const QTextFormat &v)
{
    QJsonObject propertiesObject;

    for (auto it = v.properties().cbegin(); it != v.properties().cend(); ++it) {
        propertiesObject[QString::number(it.key())] = QJsonValue::fromVariant(it.value());
    }

    QJsonObject valueObject;

    valueObject["objectIndex"] = v.objectIndex();
    valueObject["type"] = v.type();
    valueObject["properties"] = propertiesObject;

    return valueObject;
}

template<>
QTextFormat jsonToBasicType<QTextFormat>(const QJsonValue &j)
{
    auto valueObject = j.toObject();

    QTextFormat format;

    format.setObjectIndex(valueObject["objectIndex"].toInt());

    auto propertiesObject = valueObject["properties"].toObject();

    for (auto it = propertiesObject.begin(); it != propertiesObject.end(); ++it) {
        format.setProperty(it.key().toInt(), it.value().toVariant());
    }

    return format;
}

// QTransform
template<>
QJsonValue basicTypeToJson<QTransform>(const QTransform &v)
{
    QJsonArray array;

    array.append(v.m11());
    array.append(v.m12());
    array.append(v.m13());

    array.append(v.m21());
    array.append(v.m22());
    array.append(v.m23());

    array.append(v.m31());
    array.append(v.m32());
    array.append(v.m33());

    return array;
}

template<>
QTransform jsonToBasicType<QTransform>(const QJsonValue &j)
{
    auto array = j.toArray();

    return QTransform{array[0].toDouble(),
                      array[1].toDouble(),
                      array[2].toDouble(),

                      array[3].toDouble(),
                      array[4].toDouble(),
                      array[5].toDouble(),

                      array[6].toDouble(),
                      array[7].toDouble(),
                      array[8].toDouble()};
}

// QColorSpace
template<>
QJsonValue basicTypeToJson<QColorSpace>(const QColorSpace &v)
{
    return QJsonValue{QString::fromLatin1(v.iccProfile().toBase64())};
}

template<>
QColorSpace jsonToBasicType<QColorSpace>(const QJsonValue &j)
{
    return QColorSpace::fromIccProfile(QByteArray::fromBase64(j.toString().toLatin1()));
}

// QEasingCurve
template<>
QJsonValue basicTypeToJson<QEasingCurve>(const QEasingCurve &v)
{
    return QJsonValue{static_cast<int>(v.type())};
}
template<>
QEasingCurve jsonToBasicType<QEasingCurve>(const QJsonValue &v)
{
    return QEasingCurve{static_cast<QEasingCurve::Type>(v.toInt())};
}

} // namespace JsonSerializer::Impl