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
RESOURCES += resources/TurnFixStatusChanger.qrc
RC_FILE = turnfix.rc
HEADERS += src/_global.h \
    src/win_changer.h \
    src/dlg_select_club.h \
    src/dlg_tool_login.h
SOURCES += src/_global.cpp \
    src/main_changer.cpp \
    src/win_changer.cpp \
    src/dlg_select_club.cpp \
    src/dlg_tool_login.cpp
macx { 
    TARGET = TurnFixStatusChanger
    ICON += resources/icon.icns
    OBJECTS_DIR += build/o/mac
}
unix { 
    TARGET = turnfixstatuschanger
    OBJECTS_DIR += build/o/unix
    target.path += /usr/bin/
    INSTALLS += target
}
win32 { 
    TARGET = TurnFixStatusChanger
    OBJECTS_DIR += build/o/win32
    CONFIG -= debug_and_release
}
FORMS += ui/win_changer.ui \
    ui/dlg_tool_login.ui
UI_DIR += build/ui
