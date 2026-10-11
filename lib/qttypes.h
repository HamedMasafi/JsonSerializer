#pragma once

#include <QBitArray>
#include <QBitmap>
#include <QBrush>
#include <QByteArray>
#include <QByteArrayList>
#include <QColor>
#include <QColorSpace>
#include <QDataStream>
#include <QDate>
#include <QDateTime>
#include <QEasingCurve>
#include <QFont>
#include <QIcon>
#include <QImage>
#include <QJsonValue>
#include <QKeySequence>
#include <QLine>
#include <QLineF>
#include <QLocale>
#include <QPalette>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QPolygon>
#include <QPolygonF>
#include <QRect>
#include <QRectF>
#include <QRegion>
#include <QRegularExpression>
#include <QSize>
#include <QSizeF>
#include <QTextFormat>
#include <QTime>
#include <QTransform>
#include <QUrl>
#include <QUuid>

#define DECLARE_CONVERTER(Type) \
    template<> \
    QJsonValue basicTypeToJson<Type>(const Type &v); \
    template<> \
    Type jsonToBasicType<Type>(const QJsonValue &v); \
    template<> \
    struct IsQtTypeType<Type> : std::true_type \
    {};

namespace JsonSerializer::Impl {

template<typename T>
struct IsQtTypeType : std::false_type
{};

// --- TEMPLATE DECLARATIONS ---

template<typename T>
QJsonValue basicTypeToJson(const T &v);

template<typename T>
T jsonToBasicType(const QJsonValue &obj);

DECLARE_CONVERTER(bool)
DECLARE_CONVERTER(int)
DECLARE_CONVERTER(double)
DECLARE_CONVERTER(uint)
DECLARE_CONVERTER(qint64)
DECLARE_CONVERTER(quint64)
DECLARE_CONVERTER(long)
DECLARE_CONVERTER(short)
DECLARE_CONVERTER(char)
DECLARE_CONVERTER(ulong)
DECLARE_CONVERTER(ushort)
DECLARE_CONVERTER(uchar)
DECLARE_CONVERTER(float)
DECLARE_CONVERTER(signed char)
DECLARE_CONVERTER(QChar)
DECLARE_CONVERTER(QString)
DECLARE_CONVERTER(QByteArray)
DECLARE_CONVERTER(QDate)
DECLARE_CONVERTER(QTime)
DECLARE_CONVERTER(QDateTime)
DECLARE_CONVERTER(QUrl)
DECLARE_CONVERTER(QPoint)
DECLARE_CONVERTER(QPointF)
DECLARE_CONVERTER(QSize)
DECLARE_CONVERTER(QSizeF)
DECLARE_CONVERTER(QRect)
DECLARE_CONVERTER(QRectF)
DECLARE_CONVERTER(QLine)
DECLARE_CONVERTER(QLineF)
DECLARE_CONVERTER(QUuid)
DECLARE_CONVERTER(QColor)
DECLARE_CONVERTER(QPolygon)
DECLARE_CONVERTER(QPolygonF)
DECLARE_CONVERTER(QBitArray)
DECLARE_CONVERTER(QImage)
DECLARE_CONVERTER(QPixmap)
DECLARE_CONVERTER(QIcon)
DECLARE_CONVERTER(QRegion)
DECLARE_CONVERTER(QFont)
DECLARE_CONVERTER(QRegularExpression)
DECLARE_CONVERTER(QByteArrayList)
DECLARE_CONVERTER(QBrush)
DECLARE_CONVERTER(QPalette)
DECLARE_CONVERTER(QPen)
DECLARE_CONVERTER(QBitmap)
DECLARE_CONVERTER(QKeySequence)
DECLARE_CONVERTER(QTextFormat)
DECLARE_CONVERTER(QTransform)
DECLARE_CONVERTER(QColorSpace)
DECLARE_CONVERTER(QEasingCurve)

} // namespace JsonSerializer::Impl