#include "variant.h"

#include <QDataStream>
#include <QIODevice>
#include <QMatrix4x4>
#include <QMetaAssociation>
#include <QMetaSequence>
#include <QSequentialIterable>

#include "jsonserialize.h"
#include "object.h"
#include "qtcontainers.h"
#include "qttypes.h"

namespace JsonSerializer::Impl {

/*
template<template<typename> class Container>
QVariant createContainer(QMetaType::Type type)
{
    if (type == QMetaType::Bool)
        return QVariant::fromValue(Container<bool>{});
    if (type == QMetaType::Int)
        return QVariant::fromValue(Container<int>{});
    if (type == QMetaType::UInt)
        return QVariant::fromValue(Container<uint>{});
    if (type == QMetaType::LongLong)
        return QVariant::fromValue(Container<qint64>{});
    if (type == QMetaType::ULongLong)
        return QVariant::fromValue(Container<quint64>{});
    if (type == QMetaType::Double)
        return QVariant::fromValue(Container<double>{});
    if (type == QMetaType::Long)
        return QVariant::fromValue(Container<long>{});
    if (type == QMetaType::Short)
        return QVariant::fromValue(Container<short>{});
    if (type == QMetaType::Char)
        return QVariant::fromValue(Container<char>{});
    if (type == QMetaType::ULong)
        return QVariant::fromValue(Container<ulong>{});
    if (type == QMetaType::UShort)
        return QVariant::fromValue(Container<ushort>{});
    if (type == QMetaType::UChar)
        return QVariant::fromValue(Container<uchar>{});
    if (type == QMetaType::Float)
        return QVariant::fromValue(Container<float>{});
    if (type == QMetaType::SChar)
        return QVariant::fromValue(Container<signed char>{});
    if (type == QMetaType::QString)
        return QVariant::fromValue(Container<QString>{});
    if (type == QMetaType::QStringList)
        return QVariant::fromValue(Container<QStringList>{});
    if (type == QMetaType::QByteArray)
        return QVariant::fromValue(Container<QByteArray>{});
    if (type == QMetaType::QDate)
        return QVariant::fromValue(Container<QDate>{});
    if (type == QMetaType::QTime)
        return QVariant::fromValue(Container<QTime>{});
    if (type == QMetaType::QDateTime)
        return QVariant::fromValue(Container<QDateTime>{});
    if (type == QMetaType::QUrl)
        return QVariant::fromValue(Container<QUrl>{});
    if (type == QMetaType::QRect)
        return QVariant::fromValue(Container<QRect>{});
    if (type == QMetaType::QRectF)
        return QVariant::fromValue(Container<QRectF>{});
    if (type == QMetaType::QSize)
        return QVariant::fromValue(Container<QSize>{});
    if (type == QMetaType::QSizeF)
        return QVariant::fromValue(Container<QSizeF>{});
    if (type == QMetaType::QLine)
        return QVariant::fromValue(Container<QLine>{});
    if (type == QMetaType::QLineF)
        return QVariant::fromValue(Container<QLineF>{});
    if (type == QMetaType::QPoint)
        return QVariant::fromValue(Container<QPoint>{});
    if (type == QMetaType::QPointF)
        return QVariant::fromValue(Container<QPointF>{});
    if (type == QMetaType::QUuid)
        return QVariant::fromValue(Container<QUuid>{});
    if (type == QMetaType::QColor)
        return QVariant::fromValue(Container<QColor>{});
    if (type == QMetaType::QFont)
        return QVariant::fromValue(Container<QFont>{});
    if (type == QMetaType::QPixmap)
        return QVariant::fromValue(Container<QPixmap>{});
    if (type == QMetaType::QImage)
        return QVariant::fromValue(Container<QImage>{});
    if (type == QMetaType::QIcon)
        return QVariant::fromValue(Container<QIcon>{});
    if (type == QMetaType::QBrush)
        return QVariant::fromValue(Container<QBrush>{});
    if (type == QMetaType::QPen)
        return QVariant::fromValue(Container<QPen>{});
    if (type == QMetaType::QPalette)
        return QVariant::fromValue(Container<QPalette>{});
    if (type == QMetaType::QBitArray)
        return QVariant::fromValue(Container<QBitArray>{});
    if (type == QMetaType::QPolygon)
        return QVariant::fromValue(Container<QPolygon>{});
    if (type == QMetaType::QPolygonF)
        return QVariant::fromValue(Container<QPolygonF>{});
    if (type == QMetaType::QRegion)
        return QVariant::fromValue(Container<QRegion>{});
    if (type == QMetaType::QBitmap)
        return QVariant::fromValue(Container<QBitmap>{});
    if (type == QMetaType::QKeySequence)
        return QVariant::fromValue(Container<QKeySequence>{});
    if (type == QMetaType::QTextFormat)
        return QVariant::fromValue(Container<QTextFormat>{});
    if (type == QMetaType::QTransform)
        return QVariant::fromValue(Container<QTransform>{});
    if (type == QMetaType::QRegularExpression)
        return QVariant::fromValue(Container<QRegularExpression>{});
    if (type == QMetaType::QEasingCurve)
        return QVariant::fromValue(Container<QEasingCurve>{});
    if (type == QMetaType::QByteArrayList)
        return QVariant::fromValue(Container<QByteArrayList>{});
    if (type == QMetaType::QColorSpace)
        return QVariant::fromValue(Container<QColorSpace>{});
    if (type == QMetaType::QChar)
        return QVariant::fromValue(Container<QChar>{});
    if (type == QMetaType::Float16)
        return QVariant::fromValue(Container<qfloat16>{});
    if (type == QMetaType::QLocale)
        return QVariant::fromValue(Container<QLocale>{});
    if (type == QMetaType::QMatrix4x4)
        return QVariant::fromValue(Container<QMatrix4x4>{});
    if (type == QMetaType::QVariantList)
        return QVariant::fromValue(Container<QVariantList>{});
    if (type == QMetaType::QVariantMap)
        return QVariant::fromValue(Container<QVariantMap>{});
    if (type == QMetaType::QVariantHash)
        return QVariant::fromValue(Container<QVariantHash>{});
    return {};
}
*/
QString toBase64(const QVariant &v)
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds << v;
    return QString::fromLatin1(ba.toBase64());
}

QVariant jsonToVariant(const QJsonObject &obj)
{
    using namespace Impl;
    QString typeName = obj.value(options.key).toString();
    QJsonValue v = obj.value(options.value);

    // clang-format off
    if (typeName == "Bool") return QVariant::fromValue(fromJson<bool>(v));
    if (typeName == "Int") return QVariant::fromValue(fromJson<int>(v));
    if (typeName == "UInt") return QVariant::fromValue(fromJson<uint>(v));
    if (typeName == "LongLong") return QVariant::fromValue(fromJson<qint64>(v));
    if (typeName == "ULongLong") return QVariant::fromValue(fromJson<quint64>(v));
    if (typeName == "Double") return QVariant::fromValue(fromJson<double>(v));
    if (typeName == "Long") return QVariant::fromValue(fromJson<long>(v));
    if (typeName == "Short") return QVariant::fromValue(fromJson<short>(v));
    if (typeName == "Char") return QVariant::fromValue(fromJson<char>(v));
    if (typeName == "ULong") return QVariant::fromValue(fromJson<ulong>(v));
    if (typeName == "UShort") return QVariant::fromValue(fromJson<ushort>(v));
    if (typeName == "UChar") return QVariant::fromValue(fromJson<uchar>(v));
    if (typeName == "Float") return QVariant::fromValue(fromJson<float>(v));
    if (typeName == "SChar") return QVariant::fromValue(fromJson<signed char>(v));
    if (typeName == "QString") return QVariant::fromValue(fromJson<QString>(v));
    if (typeName == "QStringList") return QVariant::fromValue(fromJson<QStringList>(v));
    if (typeName == "QByteArray") return QVariant::fromValue(fromJson<QByteArray>(v));
    if (typeName == "QDate") return QVariant::fromValue(fromJson<QDate>(v));
    if (typeName == "QTime") return QVariant::fromValue(fromJson<QTime>(v));
    if (typeName == "QDateTime") return QVariant::fromValue(fromJson<QDateTime>(v));
    if (typeName == "QUrl") return QVariant::fromValue(fromJson<QUrl>(v));
    if (typeName == "QRect") return QVariant::fromValue(fromJson<QRect>(v));
    if (typeName == "QRectF") return QVariant::fromValue(fromJson<QRectF>(v));
    if (typeName == "QSize") return QVariant::fromValue(fromJson<QSize>(v));
    if (typeName == "QSizeF") return QVariant::fromValue(fromJson<QSizeF>(v));
    if (typeName == "QLine") return QVariant::fromValue(fromJson<QLine>(v));
    if (typeName == "QLineF") return QVariant::fromValue(fromJson<QLineF>(v));
    if (typeName == "QPoint") return QVariant::fromValue(fromJson<QPoint>(v));
    if (typeName == "QPointF") return QVariant::fromValue(fromJson<QPointF>(v));
    if (typeName == "QUuid") return QVariant::fromValue(fromJson<QUuid>(v));
    if (typeName == "QColor") return QVariant::fromValue(fromJson<QColor>(v));
    if (typeName == "QFont") return QVariant::fromValue(fromJson<QFont>(v));
    if (typeName == "QPixmap") return QVariant::fromValue(fromJson<QPixmap>(v));
    if (typeName == "QImage") return QVariant::fromValue(fromJson<QImage>(v));
    if (typeName == "QIcon") return QVariant::fromValue(fromJson<QIcon>(v));
    if (typeName == "QBrush") return QVariant::fromValue(fromJson<QBrush>(v));
    if (typeName == "QPen") return QVariant::fromValue(fromJson<QPen>(v));
    if (typeName == "QPalette") return QVariant::fromValue(fromJson<QPalette>(v));
    if (typeName == "QBitArray") return QVariant::fromValue(fromJson<QBitArray>(v));
    if (typeName == "QPolygon") return QVariant::fromValue(fromJson<QPolygon>(v));
    if (typeName == "QPolygonF") return QVariant::fromValue(fromJson<QPolygonF>(v));
    if (typeName == "QRegion") return QVariant::fromValue(fromJson<QRegion>(v));
    if (typeName == "QBitmap") return QVariant::fromValue(fromJson<QBitmap>(v));
    if (typeName == "QKeySequence") return QVariant::fromValue(fromJson<QKeySequence>(v));
    if (typeName == "QTextFormat") return QVariant::fromValue(fromJson<QTextFormat>(v));
    if (typeName == "QTransform") return QVariant::fromValue(fromJson<QTransform>(v));
    if (typeName == "QRegularExpression") return QVariant::fromValue(fromJson<QRegularExpression>(v));
    if (typeName == "QEasingCurve") return QVariant::fromValue(fromJson<QEasingCurve>(v));
    if (typeName == "QByteArrayList") return QVariant::fromValue(fromJson<QByteArrayList>(v));
    if (typeName == "QColorSpace") return QVariant::fromValue(fromJson<QColorSpace>(v));

    if (typeName == "bool") return QVariant::fromValue(fromJson<bool>(v));
    if (typeName == "int") return QVariant::fromValue(fromJson<int>(v));
    if (typeName == "double") return QVariant::fromValue(fromJson<double>(v));
    if (typeName == "uchar") return QVariant::fromValue(fromJson<uchar>(v));
    // clang-format on

    qDebug() << "Unable to deserialize" << typeName << obj;
    return {};
}

QVariant unpackJsonToVariant(const QJsonValue &v)
{
    auto o = v.toObject();
    auto type = QMetaType::type(o[options.key].toString().toLatin1());
    auto val = o[options.value];
    return jsonToVariant(val, static_cast<QMetaType::Type>(type));
}

QJsonValue packVariantToJson(const QVariant &val)
{
    // clang-format off
    return QJsonObject {
        {options.key, QMetaType(val.userType()).name()},
        {options.value, variantToJson(val)}
    };
    // clang-format on
}

QJsonValue variantToJson(const QVariant &val)
{
    using namespace Impl;
    if (!val.isValid() || val.isNull())
        return QJsonObject();

    int typeId = val.userType();
    QJsonValue v;
    // clang-format off
    switch (typeId) {
    case QMetaType::UnknownType: v = QJsonValue{}; break;
    case QMetaType::Nullptr: v = QJsonValue{}; break;
    case QMetaType::Bool: v = toJson(val.toBool()); break;
    case QMetaType::Int: v = toJson(val.toInt()); break;
    case QMetaType::UInt: v = toJson(val.toUInt()); break;
    case QMetaType::LongLong: v = toJson(val.toLongLong()); break;
    case QMetaType::ULongLong: v = toJson(val.toULongLong()); break;
    case QMetaType::Double: v = toJson(val.toDouble()); break;
    case QMetaType::Long: v = toJson(val.value<long>()); break;
    case QMetaType::Short: v = toJson(val.value<short>()); break;
    case QMetaType::Char: v = toJson(val.value<char>()); break;
    case QMetaType::ULong: v = toJson(val.value<ulong>()); break;
    case QMetaType::UShort: v = toJson(val.value<ushort>()); break;
    case QMetaType::UChar: v = toJson(val.value<uchar>()); break;
    case QMetaType::Float: v = toJson(val.value<float>()); break;
    case QMetaType::SChar: v = toJson(val.value<signed char>()); break;
    case QMetaType::QString: v = toJson(val.toString()); break;
    case QMetaType::QStringList: v = toJson(val.toStringList()); break;
    case QMetaType::QByteArray: v = toJson(val.toByteArray()); break;
    case QMetaType::QDate: v = toJson(val.toDate()); break;
    case QMetaType::QTime: v = toJson(val.toTime()); break;
    case QMetaType::QDateTime: v = toJson(val.toDateTime()); break;
    case QMetaType::QUrl: v = toJson(val.toUrl()); break;
    case QMetaType::QRect: v = toJson(val.toRect()); break;
    case QMetaType::QRectF: v = toJson(val.toRectF()); break;
    case QMetaType::QSize: v = toJson(val.toSize()); break;
    case QMetaType::QSizeF: v = toJson(val.toSizeF()); break;
    case QMetaType::QLine: v = toJson(val.toLine()); break;
    case QMetaType::QLineF: v = toJson(val.toLineF()); break;
    case QMetaType::QPoint: v = toJson(val.toPoint()); break;
    case QMetaType::QPointF: v = toJson(val.toPointF()); break;
    case QMetaType::QUuid: v = toJson(val.value<QUuid>()); break;
    case QMetaType::QColor: v = toJson(val.value<QColor>()); break;
    case QMetaType::QFont: v = toJson(val.value<QFont>()); break;
    case QMetaType::QPixmap: v = toJson(val.value<QPixmap>()); break;
    case QMetaType::QImage: v = toJson(val.value<QImage>()); break;
    case QMetaType::QIcon: v = toJson(val.value<QIcon>()); break;
    case QMetaType::QBrush: v = toJson(val.value<QBrush>()); break;
    case QMetaType::QPen: v = toJson(val.value<QPen>()); break;
    case QMetaType::QPalette: v = toJson(val.value<QPalette>()); break;
    case QMetaType::QBitArray: v = toJson(val.value<QBitArray>()); break;
    case QMetaType::QPolygon: v = toJson(val.value<QPolygon>()); break;
    case QMetaType::QPolygonF: v = toJson(val.value<QPolygonF>()); break;
    case QMetaType::QRegion: v = toJson(val.value<QRegion>()); break;
    case QMetaType::QBitmap: v = toJson(val.value<QBitmap>()); break;
    case QMetaType::QKeySequence: v = toJson(val.value<QKeySequence>()); break;
    case QMetaType::QTextFormat: v = toJson(val.value<QTextFormat>()); break;
    case QMetaType::QTransform: v = toJson(val.value<QTransform>()); break;
    case QMetaType::QRegularExpression: v = toJson(val.value<QRegularExpression>()); break;
    case QMetaType::QEasingCurve: v = toJson(val.value<QEasingCurve>()); break;
    case QMetaType::QByteArrayList: v = toJson(val.value<QByteArrayList>()); break;
    case QMetaType::QColorSpace: v = toJson(val.value<QColorSpace>()); break;
    case QMetaType::QChar: v = toJson(val.value<QChar>()); break;
    case QMetaType::Float16: v = toJson(val.value<qfloat16>()); break;
    case QMetaType::QLocale: v = toJson(val.value<QLocale>()); break;
    case QMetaType::QMatrix4x4: v = toJson(val.value<QMatrix4x4>()); break;
    case QMetaType::QVariantList: v = listToJson<QVariantList>(val.toList()); break;
    case QMetaType::QVariantMap: v = mapToJson<QVariantMap>(val.toMap()); break;
    case QMetaType::QVariantHash: v = hashToJson<QVariantHash>(val.toHash()); break;
    // clang-format on
    default: {
        auto mt = QMetaType(typeId);
        if (mt.flags() & QMetaType::IsEnumeration) {
            qDebug() << "Is enum:" << mt.underlyingType() << mt.name() << mt.id();
            // v = variantToJson(val, mt.underlyingType());
            v = toJson(val.toInt());
        } else if (mt.flags() & QMetaType::PointerToGadget) {
            return gadgetToJson(val.data(), *mt.metaObject());
        } else {
            QString name = QMetaType(typeId).name();
            if (val.canConvert<QVariantList>()) {
                QJsonArray arr;

                auto iterable = val.value<QMetaSequence::Iterable>();

                for (const QVariant &item : iterable) {
                    arr.append(variantToJson(item));
                }
                // qDebug() << name << "Converted to list";
                return arr;
            } else if (val.canConvert<QMetaAssociation::Iterable>()) {
                auto iterable = val.value<QMetaAssociation::Iterable>();
                QJsonArray arr;
                for (auto it = iterable.begin(); it != iterable.end(); ++it) {
                    const QVariant key = it.key();
                    const QVariant value = it.value();

                    QJsonObject o;
                    o[options.key] = variantToJson(key);
                    o[options.value] = variantToJson(value);
                    arr.append(o);
                }
                // qDebug() << name << "Converted to QMetaAssociation::Iterable";

                return arr;
            }

            if (name.startsWith("QList<")) {
                return variantToJson(val.value<QVariantList>());
            } else if (name.startsWith("QMap<")) {
                return variantToJson(val.value<QVariantMap>());
            } else if (name.startsWith("QSet<")) {
                // return variantToJson(val.value<QSet<QVariant>>());
            } else if (name.startsWith("QHash<")) {
                return variantToJson(val.value<QVariantHash>());
            } else if (name.startsWith("QStack<")) {
                return variantToJson(val.value<QStack<QVariant>>());
            } else if (name.startsWith("QQueue<")) {
                // return variantToJson(val.value<QQueue<QVariant>>());
            }
            // Fallback for types not explicitly handled
            qWarning() << Q_FUNC_INFO << "Unable to save" << typeId << QMetaType(typeId).name()
                       << val.canConvert<QVariantList>();
            return {}; // {{options.key, "Unknown"}, {options.value, toBase64(val)}};
        }
    }
    }

    return v; //QJsonObject{{options.key, QString{QMetaType(typeId).name()}}, {options.value, v}};
}

QVariant jsonToVariant(const QJsonValue &v, QMetaType::Type type)
{
    switch (type) {
        // clang-format off
    case QMetaType::UnknownType: return QVariant{}; break;
    case QMetaType::Nullptr: return QVariant::fromValue(nullptr); break;
    case QMetaType::Bool: return QVariant::fromValue(fromJson<bool>(v));
    case QMetaType::Int: return QVariant::fromValue(fromJson<int>(v));
    case QMetaType::UInt: return QVariant::fromValue(fromJson<uint>(v));
    case QMetaType::LongLong: return QVariant::fromValue(fromJson<qint64>(v));
    case QMetaType::ULongLong: return QVariant::fromValue(fromJson<quint64>(v));
    case QMetaType::Double: return QVariant::fromValue(fromJson<double>(v));
    case QMetaType::Long: return QVariant::fromValue(fromJson<long>(v));
    case QMetaType::Short: return QVariant::fromValue(fromJson<short>(v));
    case QMetaType::Char: return QVariant::fromValue(fromJson<char>(v));
    case QMetaType::ULong: return QVariant::fromValue(fromJson<ulong>(v));
    case QMetaType::UShort: return QVariant::fromValue(fromJson<ushort>(v));
    case QMetaType::UChar: return QVariant::fromValue(fromJson<uchar>(v));
    case QMetaType::Float: return QVariant::fromValue(fromJson<float>(v));
    case QMetaType::SChar: return QVariant::fromValue(fromJson<signed char>(v));
    case QMetaType::QString: return QVariant::fromValue(fromJson<QString>(v));
    case QMetaType::QStringList: return QVariant::fromValue(fromJson<QStringList>(v));
    case QMetaType::QByteArray: return QVariant::fromValue(fromJson<QByteArray>(v));
    case QMetaType::QDate: return QVariant::fromValue(fromJson<QDate>(v));
    case QMetaType::QTime: return QVariant::fromValue(fromJson<QTime>(v));
    case QMetaType::QDateTime: return QVariant::fromValue(fromJson<QDateTime>(v));
    case QMetaType::QUrl: return QVariant::fromValue(fromJson<QUrl>(v));
    case QMetaType::QRect: return QVariant::fromValue(fromJson<QRect>(v));
    case QMetaType::QRectF: return QVariant::fromValue(fromJson<QRectF>(v));
    case QMetaType::QSize: return QVariant::fromValue(fromJson<QSize>(v));
    case QMetaType::QSizeF: return QVariant::fromValue(fromJson<QSizeF>(v));
    case QMetaType::QLine: return QVariant::fromValue(fromJson<QLine>(v));
    case QMetaType::QLineF: return QVariant::fromValue(fromJson<QLineF>(v));
    case QMetaType::QPoint: return QVariant::fromValue(fromJson<QPoint>(v));
    case QMetaType::QPointF: return QVariant::fromValue(fromJson<QPointF>(v));
    case QMetaType::QUuid: return QVariant::fromValue(fromJson<QUuid>(v));
    case QMetaType::QColor: return QVariant::fromValue(fromJson<QColor>(v));
    case QMetaType::QFont: return QVariant::fromValue(fromJson<QFont>(v));
    case QMetaType::QPixmap: return QVariant::fromValue(fromJson<QPixmap>(v));
    case QMetaType::QImage: return QVariant::fromValue(fromJson<QImage>(v));
    case QMetaType::QIcon: return QVariant::fromValue(fromJson<QIcon>(v));
    case QMetaType::QBrush: return QVariant::fromValue(fromJson<QBrush>(v));
    case QMetaType::QPen: return QVariant::fromValue(fromJson<QPen>(v));
    case QMetaType::QPalette: return QVariant::fromValue(fromJson<QPalette>(v));
    case QMetaType::QBitArray: return QVariant::fromValue(fromJson<QBitArray>(v));
    case QMetaType::QPolygon: return QVariant::fromValue(fromJson<QPolygon>(v));
    case QMetaType::QPolygonF: return QVariant::fromValue(fromJson<QPolygonF>(v));
    case QMetaType::QRegion: return QVariant::fromValue(fromJson<QRegion>(v));
    case QMetaType::QBitmap: return QVariant::fromValue(fromJson<QBitmap>(v));
    case QMetaType::QKeySequence: return QVariant::fromValue(fromJson<QKeySequence>(v));
    case QMetaType::QTextFormat: return QVariant::fromValue(fromJson<QTextFormat>(v));
    case QMetaType::QTransform: return QVariant::fromValue(fromJson<QTransform>(v));
    case QMetaType::QRegularExpression: return QVariant::fromValue(fromJson<QRegularExpression>(v));
    case QMetaType::QEasingCurve: return QVariant::fromValue(fromJson<QEasingCurve>(v));
    case QMetaType::QByteArrayList: return QVariant::fromValue(fromJson<QByteArrayList>(v));
    case QMetaType::QColorSpace: return QVariant::fromValue(fromJson<QColorSpace>(v));
    case QMetaType::QChar: return QVariant::fromValue(fromJson<QChar>(v)); break;
    case QMetaType::Float16: return QVariant::fromValue(fromJson<qfloat16>(v)); break;
    case QMetaType::QLocale: return QVariant::fromValue(fromJson<QLocale>(v)); break;
    case QMetaType::QMatrix4x4: return QVariant::fromValue(fromJson<QMatrix4x4>(v)); break;
    case QMetaType::QVariantList: return QVariant::fromValue(jsonToList<QVariantList>(v)); break;
    case QMetaType::QVariantMap: return QVariant::fromValue(jsonToMap<QVariantMap>(v)); break;
    case QMetaType::QVariantHash: return QVariant::fromValue(jsonToHash<QVariantHash>(v)); break;
    // clang-format on
    default: {
        auto mt = QMetaType(type);
        if (mt.flags() & QMetaType::IsEnumeration) {
            return fromJson<int>(v);
        } else if (mt.flags() & QMetaType::PointerToGadget) {
            auto g = createGadget(mt.name());
            jsonToGadget(v.toObject(), g.data(), *mt.metaObject());
            return g;
        } else {

            QString name = QMetaType(type).name();
            if (QMetaType::canConvert(mt, QMetaType::fromType<QMetaAssociation::Iterable>())) {
                auto tmp = jsonToAssociation(v, type);
                return tmp;
            }
            if (QMetaType::canConvert(mt, QMetaType::fromType<QMetaSequence::Iterable>())) {
                auto tmp = jsonToVariantList(v, type);
                return tmp;
            }
            // if (QMetaType::canConvert(mt, QMetaType::fromType<QSet<QVariant>>()))
                // return jsonToVariantList(v, type);

            // Fallback for types not explicitly handled
            qWarning() << Q_FUNC_INFO << "Unable to load" << type << name;
        }
    }
    }
    return {};
}

QVariant jsonToVariantList(const QJsonValue &json, QMetaType::Type type)
{
    auto mt = QMetaType{type};
    if (!json.isArray())
        return {};

    auto var = QVariant::fromMetaType(mt);
    auto arr = json.toArray();

    auto it = var.view<QMetaSequence::Iterable>();
    QMetaType valueMeta = it.metaContainer().valueMetaType();

    const QMetaSequence meta = it.metaContainer();
    void *mutableData = it.mutableIterable();

    for (const QJsonValue &v : arr) {
        QVariant item = jsonToVariant(v, static_cast<QMetaType::Type>(valueMeta.id()));
        meta.addValue(mutableData, item.data());
    }

    return var;
}

QVariant jsonToAssociation(const QJsonValue &json, QMetaType::Type type) {
    auto mt = QMetaType{type};

    auto var = QVariant::fromMetaType(mt);

    auto it = var.view<QMetaAssociation::Iterable>();
    auto hasKeys = it.metaContainer().canContainsKey();

    QMetaType keyMeta = it.metaContainer().keyMetaType();
    QMetaType valueMeta = it.metaContainer().mappedMetaType();

    if (json.isArray()) {
        auto arr = json.toArray();
        for (const QJsonValue &v : arr) {
            if (hasKeys) {
                auto o = v.toObject();
                QVariant key = jsonToVariant(o[options.key],
                                             static_cast<QMetaType::Type>(keyMeta.id()));
                QVariant value = jsonToVariant(o[options.value],
                                               static_cast<QMetaType::Type>(valueMeta.id()));
                it.setValue(key, value);
            } else {
                auto vvv =jsonToVariant(v, static_cast<QMetaType::Type>(keyMeta.id()));
                it.insertKey(vvv);
            }
        }
    } else if (json.isObject()) {
    }

    return var;
}

} // namespace JsonSerializer::Impl