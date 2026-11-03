#pragma once

#include <QBitArray>
#include <QBitmap>
#include <QBrush>
#include <QByteArrayList>
#include <QColor>
#include <QColorSpace>
#include <QDate>
#include <QDateTime>
#include <QEasingCurve>
#include <QFont>
#include <QHash>
#include <QIcon>
#include <QImage>
#include <QKeySequence>
#include <QLine>
#include <QLineF>
#include <QList>
#include <QLocale>
#include <QMap>
#include <QMatrix4x4>
#include <QMetaType>
#include <QObject>
#include <QPalette>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QPolygon>
#include <QPolygonF>
#include <QQueue>
#include <QRandomGenerator>
#include <QRect>
#include <QRectF>
#include <QRegion>
#include <QRegularExpression>
#include <QSet>
#include <QSize>
#include <QSizeF>
#include <QStack>
#include <QTest>
#include <QTextFormat>
#include <QTime>
#include <QTransform>
#include <QUrl>
#include <QUuid>
#include <QVector>

class TestObject : public QObject
{
    Q_OBJECT

    // ---------- Basic numeric / character types ----------
    Q_PROPERTY(bool p_bool MEMBER p_bool)
    Q_PROPERTY(char p_char MEMBER p_char)
    Q_PROPERTY(signed char p_schar MEMBER p_schar)
    Q_PROPERTY(uchar p_uchar MEMBER p_uchar)
    Q_PROPERTY(short p_short MEMBER p_short)
    Q_PROPERTY(ushort p_ushort MEMBER p_ushort)
    Q_PROPERTY(int p_int MEMBER p_int)
    Q_PROPERTY(uint p_uint MEMBER p_uint)
    Q_PROPERTY(long p_long MEMBER p_long)
    Q_PROPERTY(ulong p_ulong MEMBER p_ulong)
    Q_PROPERTY(qint64 p_qint64 MEMBER p_qint64)
    Q_PROPERTY(quint64 p_quint64 MEMBER p_quint64)
    Q_PROPERTY(float p_float MEMBER p_float)
    Q_PROPERTY(double p_double MEMBER p_double)
    Q_PROPERTY(qfloat16 p_qfloat16 MEMBER p_qfloat16)

    // ---------- Qt value types ----------
    Q_PROPERTY(QChar p_qchar MEMBER p_qchar)
    Q_PROPERTY(QString p_string MEMBER p_string)
    Q_PROPERTY(QByteArray p_bytearray MEMBER p_bytearray)
    Q_PROPERTY(QDate p_date MEMBER p_date)
    Q_PROPERTY(QTime p_time MEMBER p_time)
    Q_PROPERTY(QDateTime p_datetime MEMBER p_datetime)
    Q_PROPERTY(QUrl p_url MEMBER p_url)
    Q_PROPERTY(QUuid p_uuid MEMBER p_uuid)
    Q_PROPERTY(QColor p_color MEMBER p_color)
    Q_PROPERTY(QPoint p_point MEMBER p_point)
    Q_PROPERTY(QPointF p_pointf MEMBER p_pointf)
    Q_PROPERTY(QSize p_size MEMBER p_size)
    Q_PROPERTY(QSizeF p_sizef MEMBER p_sizef)
    Q_PROPERTY(QRect p_rect MEMBER p_rect)
    Q_PROPERTY(QRectF p_rectf MEMBER p_rectf)
    Q_PROPERTY(QLine p_line MEMBER p_line)
    Q_PROPERTY(QLineF p_linef MEMBER p_linef)
    Q_PROPERTY(QPolygon p_polygon MEMBER p_polygon)
    Q_PROPERTY(QPolygonF p_polygonf MEMBER p_polygonf)
    Q_PROPERTY(QBitArray p_bitarray MEMBER p_bitarray)
    Q_PROPERTY(QImage p_image MEMBER p_image)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(QRegion p_region MEMBER p_region)
    Q_PROPERTY(QFont p_font MEMBER p_font)
    Q_PROPERTY(QRegularExpression p_regexp MEMBER p_regexp)
    Q_PROPERTY(QByteArrayList p_bytearraylist MEMBER p_bytearraylist)
    Q_PROPERTY(QBrush p_brush MEMBER p_brush)
    Q_PROPERTY(QPalette p_palette MEMBER p_palette)
    Q_PROPERTY(QPen p_pen MEMBER p_pen)
    Q_PROPERTY(QBitmap bitmap READ bitmap WRITE setBitmap)
    Q_PROPERTY(QKeySequence p_keysequence MEMBER p_keysequence)
    Q_PROPERTY(QTextFormat p_textformat MEMBER p_textformat)
    Q_PROPERTY(QTransform p_transform MEMBER p_transform)
    Q_PROPERTY(QColorSpace p_colorspace MEMBER p_colorspace)
    Q_PROPERTY(QEasingCurve p_easingcurve MEMBER p_easingcurve)
    Q_PROPERTY(QLocale p_locale MEMBER p_locale)
    Q_PROPERTY(QMatrix4x4 p_matrix4x4 MEMBER p_matrix4x4)

    // ---------- Containers ----------
    Q_PROPERTY(QList<int> p_qlist MEMBER p_qlist)
    Q_PROPERTY(QVector<QString> p_qvector MEMBER p_qvector)
    Q_PROPERTY(QSet<int> p_qset MEMBER p_qset)
    Q_PROPERTY(QMap<QString, int> p_qmap MEMBER p_qmap)
    Q_PROPERTY(QHash<QString, double> p_qhash MEMBER p_qhash)
    Q_PROPERTY(QQueue<int> p_qqueue MEMBER p_qqueue)
    Q_PROPERTY(QStack<QString> p_qstack MEMBER p_qstack)
    Q_PROPERTY(QList<QPoint> p_qlist_point MEMBER p_qlist_point)

public:
    explicit TestObject(QObject *parent = nullptr);

    void fillRandom();

public:
    // ---------- Members ----------
    bool p_bool = false;
    char p_char = 0;
    signed char p_schar = 0;
    uchar p_uchar = 0;
    short p_short = 0;
    ushort p_ushort = 0;
    int p_int = 0;
    uint p_uint = 0;
    long p_long = 0;
    ulong p_ulong = 0;
    qint64 p_qint64 = 0;
    quint64 p_quint64 = 0;
    float p_float = 0;
    double p_double = 0;
    qfloat16 p_qfloat16 = 0;

    QChar p_qchar;
    QString p_string;
    QByteArray p_bytearray;
    QDate p_date;
    QTime p_time;
    QDateTime p_datetime;
    QUrl p_url;
    QUuid p_uuid;
    QColor p_color;
    QPoint p_point;
    QPointF p_pointf;
    QSize p_size;
    QSizeF p_sizef;
    QRect p_rect;
    QRectF p_rectf;
    QLine p_line;
    QLineF p_linef;
    QPolygon p_polygon;
    QPolygonF p_polygonf;
    QBitArray p_bitarray;
    QImage p_image;
    QPixmap p_pixmap;
    QIcon p_icon;
    QRegion p_region;
    QFont p_font;
    QRegularExpression p_regexp;
    QByteArrayList p_bytearraylist;
    QBrush p_brush;
    QPalette p_palette;
    QPen p_pen;
    QBitmap p_bitmap;
    QKeySequence p_keysequence;
    QTextFormat p_textformat;
    QTransform p_transform;
    QColorSpace p_colorspace;
    QEasingCurve p_easingcurve;
    QLocale p_locale;
    QMatrix4x4 p_matrix4x4;

    QList<int> p_qlist;
    QVector<QString> p_qvector;
    QSet<int> p_qset;
    QMap<QString, int> p_qmap;
    QHash<QString, double> p_qhash;
    QQueue<int> p_qqueue;
    QStack<QString> p_qstack;
    QList<QPoint> p_qlist_point;
    QPixmap pixmap() const;
    void setPixmap(const QPixmap &newPixmap);

    QIcon icon() const;
    void setIcon(const QIcon &newIcon);

    QBitmap bitmap() const;
    void setBitmap(const QBitmap &newBitmap);

private:
    QPixmap m_pixmap;
    QIcon m_icon;
    QBitmap m_bitmap;
};