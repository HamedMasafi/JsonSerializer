#pragma once

#include <QJsonObject>

namespace JsonSerializer::Impl {

QJsonObject objectToJson(QObject *obj);
bool jsonToObject(const QJsonObject &json, QObject *obj);
QObject *createQObject(const QString &className);

QJsonObject gadgetToJson(const void *obj, const QMetaObject &metaObject);
bool jsonToGadget(const QJsonObject &json, void *obj, const QMetaObject &metaObject);
QVariant createGadget(const QString &className);

} // namespace JsonSerializer::Impl
