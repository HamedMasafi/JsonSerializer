#pragma once

#include <QJsonObject>
#include <QMetaType>

namespace JsonSerializer::Impl {

QJsonValue variantToJson(const QVariant &val);
QVariant jsonToVariant(const QJsonObject &obj);
QVariant jsonToVariant(const QJsonValue &val, QMetaType::Type type);

QJsonValue packVariantToJson(const QVariant &val);
QVariant unpackJsonToVariant(const QJsonValue &val);

QVariant jsonToVariantList(const QJsonValue &val, QMetaType::Type type);
QVariant jsonToAssociation(const QJsonValue &val, QMetaType::Type type);

}
