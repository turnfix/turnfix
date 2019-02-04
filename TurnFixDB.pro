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
HEADERS += src/_global.h \
    src/win_dbase.h
SOURCES += src/main_db.cpp \
    src/_global.cpp \
    src/win_dbase.cpp
macx { 
    TARGET = TurnFixDatenbankTool
    ICON += resources/icon.icns
    OBJECTS_DIR += build/o/mac
}
unix { 
    TARGET = turnfixdatenbanktool
    OBJECTS_DIR += build/o/unix
    target.path += /usr/bin/
    INSTALLS += target
}
win32 { 
    TARGET = TurnFixDatenbankTool
    OBJECTS_DIR += build/o/win32
    CONFIG -= debug_and_release
}
FORMS += ui/win_dbase.ui
UI_DIR += build/ui
