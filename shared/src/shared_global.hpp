#ifndef SHARED_GLOBAL_HPP
#define SHARED_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(TESTLIB_LIBRARY)
#  define SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SHARED_GLOBAL_HPP
