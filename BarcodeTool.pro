CONFIG -= debug_and_release
CONFIG += qt \
    release \
    uitools \
    warn_on
DESTDIR += bin
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
MOC_DIR += build/moc
RCC_DIR += build/rcc
QT += sql \
    xml \
    network
RESOURCES += resources/TurnFix.qrc
RC_FILE = turnfix.rc
HEADERS += src/global/header/_global.h \
    src/dialogs/others/header/dlg_tool_login.h \
    src/windows/header/win_barcode.h \
    src/dialogs/participants/header/dlg_tn.h \
    src/dialogs/database/header/dlg_db_club.h \
    src/dialogs/database/header/dlg_db_person.h \
    src/dialogs/database/header/dlg_db_gau.h \
    src/dialogs/database/header/dlg_db_verband.h \
    src/dialogs/database/header/dlg_db_land.h \
    src/dialogs/database/header/dlg_db_tn.h \
    src/global/header/_delegates.h \
    src/libs/fparser/fptypes.hh \
    src/libs/fparser/fpconfig.hh \
    src/libs/fparser/fpaux.hh \
    src/libs/fparser/fparser.hh \
    src/dialogs/participants/header/dlg_group.h
SOURCES += src/global/_global.cpp \
    src/dialogs/others/dlg_tool_login.cpp \
    src/main_barcode.cpp \
    src/windows/win_barcode.cpp \
    src/dialogs/participants/dlg_tn.cpp \
    src/dialogs/database/dlg_db_club.cpp \
    src/dialogs/database/dlg_db_person.cpp \
    src/dialogs/database/dlg_db_gau.cpp \
    src/dialogs/database/dlb_db_verband.cpp \
    src/dialogs/database/dlg_db_land.cpp \
    src/dialogs/database/dlg_db_tn.cpp \
    src/global/_delegates.cpp \
    src/libs/fparser/fparser.cc \
    src/dialogs/participants/dlg_group.cpp
macx { 
    TARGET = TurnFixBarcodeTool
    ICON += resources/icon.icns
    OBJECTS_DIR += build/o/mac
}
unix { 
    TARGET = turnfixbarcodetool
    OBJECTS_DIR += build/o/unix
    target.path += /usr/bin/
    INSTALLS += target
}
win32 { 
    TARGET = TurnFixBarcodeTool
    OBJECTS_DIR += build/o/win32
    CONFIG -= debug_and_release
}
FORMS += src/dialogs/others/ui/dlg_tool_login.ui \
    src/windows/ui/win_barcode.ui \
    src/dialogs/participants/ui/dlg_tn.ui \
    src/dialogs/database/ui/dlg_db_club.ui \
    src/dialogs/database/ui/dlg_db_person.ui \
    src/dialogs/database/ui/dlg_db_gau.ui \
    src/dialogs/database/ui/dlg_db_verband.ui \
    src/dialogs/database/ui/dlg_db_land.ui \
    src/dialogs/database/ui/dlg_db_tn.ui \
    src/dialogs/participants/ui/dlg_group.ui
UI_DIR += build/ui

OTHER_FILES += \
    src/libs/fparser/fp_opcode_add.inc \
    src/libs/fparser/fp_identifier_parser.inc
