TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
QT += widgets xml sql network printsupport
TRANSLATIONS = lang/turnfix_de.ts
RC_FILE = turnfix.rc

CONFIG(release, debug|release) {
    CONFIG += release
    DESTDIR = output/release/bin
    MOC_DIR += output/release/moc
    RCC_DIR += output/release/rcc
    UI_DIR += output/release/ui
    OBJECTS_DIR = output/release/o
} else {
    CONFIG += debug
    DESTDIR = output/debug/bin
    MOC_DIR += output/debug/moc
    RCC_DIR += output/debug/rcc
    UI_DIR += output/debug/ui
    OBJECTS_DIR = output/debug/o
}

macx {
    CONFIG += x86_64
    TARGET = TurnFix
    ICON += resources/icon.icns
    LIBS += "-L/Library/PostgreSQL/11/lib" -lpq
}
win32 {
    TARGET = TurnFix
}

include(app/app.pri)
include(masterdata/masterdata.pri)
include(competitions/competitions.pri)
include(participants/participants.pri)
include(subdivisions/subdivisions.pri)
include(startingorder/startingorder.pri)
include(capture/capture.pri)
include(results/results.pri)
include(export/export.pri)
include(status/status.pri)
include(editor/editor.pri)
include(controls/controls.pri)
include(libs/libs.pri)

HEADERS += model/objects/competition.h \
    src/global/header/_delegates.h \
    src/global/header/_global.h \
    src/global/header/settings.h \
    src/global/header/result_calc.h \
    model/database/db.h \
    model/settings/tfsettings.h \
    model/objects/event.h \
    model/database/tfsqlquery.h \
    model/objects/location.h \
    model/settings/session.h
SOURCES += main.cpp \
    model/database/db.cpp \
    model/database/tfsqlquery.cpp \
    model/objects/competition.cpp \
    model/objects/event.cpp \
    model/objects/location.cpp \
    model/settings/session.cpp \
    model/settings/tfsettings.cpp \
    src/global/_delegates.cpp \
    src/global/_global.cpp \
    src/global/settings.cpp \
    src/global/result_calc.cpp
RESOURCES += resources/TurnFix.qrc \
    resources/TF_Language.qrc \
    resources/TF_GeraeteIcons.qrc \
    assets/icons/icons.qrc


DISTFILES += \
    .gitignore
