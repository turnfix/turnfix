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
RESOURCES += resources/TurnFixStatus.qrc
RC_FILE = turnfix.rc
HEADERS += src/global/header/_global.h \
    src/widgets/header/wdg_wkstatus.h \
    src/windows/header/win_statustool.h \
    src/dialogs/others/header/dlg_tool_login.h \
    src/libs/fparser/fptypes.hh \
    src/libs/fparser/fpconfig.hh \
    src/libs/fparser/fpaux.hh \
    src/libs/fparser/fparser.hh
SOURCES += src/main_status.cpp \
    src/global/_global.cpp \
    src/widgets/wdg_wkstatus.cpp \
    src/windows/win_statustool.cpp \
    src/dialogs/others/dlg_tool_login.cpp \
    src/libs/fparser/fparser.cc
macx { 
    TARGET = TurnFixStatusTool
    ICON += resources/icon.icns
    OBJECTS_DIR += build/o/mac
}
unix { 
    TARGET = turnfixstatustool
    OBJECTS_DIR += build/o/unix
    target.path += /usr/bin/
    INSTALLS += target
}
win32 { 
    TARGET = TurnFixStatusTool
    OBJECTS_DIR += build/o/win32
    CONFIG -= debug_and_release
}
FORMS += src/windows/ui/win_statustool.ui \
    src/widgets/ui/wdg_wkstatus.ui \
    src/dialogs/others/ui/dlg_tool_login.ui
UI_DIR += build/ui
