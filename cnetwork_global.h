#ifndef CNETWORK_GLOBAL_H
#define CNETWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CNETWORK_LIBRARY)
#  define CNETWORK_EXPORT Q_DECL_EXPORT
#else
#  define CNETWORK_EXPORT Q_DECL_IMPORT
#endif

#endif // CNETWORK_GLOBAL_H
