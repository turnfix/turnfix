HEADERS += \
    $$PWD/dbchecker.h \
    $$PWD/dbcolumn.h \
    $$PWD/dbconstraint.h \
    $$PWD/dbtable.h \
    $$PWD/entitymanager.h \
    $$PWD/enums.h \
    $$PWD/querybuilder.h \
    $$PWD/team.h

SOURCES += \
    $$PWD/dbchecker.cpp \
    $$PWD/dbcolumn.cpp \
    $$PWD/dbconstraint.cpp \
    $$PWD/dbtable.cpp \
    $$PWD/entitymanager.cpp \
    $$PWD/team.cpp

include(entity/entity.pri)
include(repository/repository.pri)
