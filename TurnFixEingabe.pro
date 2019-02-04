TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
QT += sql
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
    CONFIG += x86
    TARGET = "TurnFix Kampfrichter-Tool"
    ICON += resources/icon.icns
    LIBS += "-L/Library/PostgreSQL/8.4/lib" -lpq
}
win32 {
    TARGET = "TurnFixKampfrichter-Tool"
}

HEADERS += src/windows/header/win_eingabe.h \
    src/windows/header/win_display.h \
    src/widgets/header/wdg_tab_sr.h \
    src/dialogs/select/header/dlg_select_dis.h \
    src/dialogs/select/header/dlg_select_rg.h \
    src/models/header/mdl_riege.h \
    src/models/header/mdl_result.h \
    src/libs/fparser/fptypes.hh \
    src/libs/fparser/fpconfig.hh \
    src/libs/fparser/fpaux.hh \
    src/libs/fparser/fparser.hh \
    src/global/header/result_calc.h \
    src/global/header/_global.h \
    src/global/header/settings.h \
    src/global/header/_delegates.h \
    src/dialogs/others/header/dlg_tool_login.h \
    src/widgets/subclasses/header/extendedlistwidget.h
SOURCES += src/main_eingabe.cpp \
    src/windows/win_eingabe.cpp \
    src/windows/win_display.cpp \
    src/dialogs/select/dlg_select_dis.cpp \
    src/dialogs/select/dlg_select_rg.cpp \
    src/widgets/wdg_tab_sr.cpp \
    src/models/mdl_riege.cpp \
    src/models/mdl_result.cpp \
    src/libs/fparser/fparser.cc \
    src/global/result_calc.cpp \
    src/global/_global.cpp \
    src/global/settings.cpp \
    src/global/_delegates.cpp \
    src/dialogs/others/dlg_tool_login.cpp \
    src/widgets/subclasses/extendedlistwidget.cpp
FORMS += src/windows/ui/win_eingabe.ui \
    src/windows/ui/win_display.ui \
    src/dialogs/others/ui/dlg_tool_login.ui \
    src/dialogs/select/ui/dlg_select_rg.ui \
    src/dialogs/select/ui/dlg_select_dis.ui \
    src/widgets/ui/wdg_tab_sr.ui
RESOURCES += resources/TurnFix.qrc \
    resources/TF_Language.qrc \
    resources/TF_GeraeteIcons.qrc
