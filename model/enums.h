#ifndef ENUMS_H
#define ENUMS_H

#include "qnamespace.h"

namespace TF {
enum ItemDataRole { ObjectRole = Qt::UserRole, IdRole };
}

enum ColumnType { SmallInt, Integer, Boolean, Varchar, Date, Real, Text, TimeNoTimezone };

#endif // ENUMS_H
