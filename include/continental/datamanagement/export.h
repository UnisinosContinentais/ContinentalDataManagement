#ifndef CONTINENTALDATAMANAGEMENT_EXPORT_H
#define CONTINENTALDATAMANAGEMENT_EXPORT_H

#include <QtCore/QtGlobal>

#if defined(MAKE_CONTINENTALDATAMANAGEMENT_LIB)
#  define CONTINENTALDATAMANAGEMENT_EXPORT Q_DECL_EXPORT
#else
#  define CONTINENTALDATAMANAGEMENT_EXPORT Q_DECL_IMPORT
#endif

#endif // CONTINENTALDATAMANAGEMENT_EXPORT_H
