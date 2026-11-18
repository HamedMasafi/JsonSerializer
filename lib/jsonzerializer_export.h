#pragma once

#include <QtCore/qglobal.h>

#if defined(JSONSERIALIZER_LIBRARY)
#  define JSONSERIALIZER_EXPORT Q_DECL_EXPORT
#else
#  define JSONSERIALIZER_EXPORT Q_DECL_IMPORT
#endif