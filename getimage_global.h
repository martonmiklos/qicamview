#ifndef GETIMAGE_GLOBAL_H
#define GETIMAGE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GETIMAGE_LIBRARY)
#  define GETIMAGESHARED_EXPORT Q_DECL_EXPORT
#else
#  define GETIMAGESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GETIMAGE_GLOBAL_H