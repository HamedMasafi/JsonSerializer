#pragma once

#include <QJsonObject>
#include <QMetaType>

namespace JsonSerializer::Impl {

QJsonValue variantToJson(const QVariant &val);
QVariant jsonToVariant(const QJsonObject &obj);
QVariant jsonToVariant(const QJsonValue &val, QMetaType::Type type);

QJsonValue packVariantToJson(const QVariant &val);
QVariant unpackJsonToVariant(const QJsonValue &val);

}
