include(misc/misc.pri)
include(participants/participants.pri)
include(results/results.pri)

FORMS += \
    $$PWD/exportwidget.ui \
    $$PWD/maildialog.ui \
    $$PWD/selectclubdialog.ui \
    $$PWD/selectcompetitiondialog.ui \
    $$PWD/selectdetaildialog.ui \
    $$PWD/selectdisciplinedialog.ui \
    $$PWD/selectparticipantdialog.ui \
    $$PWD/selectsubdivisiondialog.ui

HEADERS += \
    $$PWD/exportwidget.h \
    $$PWD/maildialog.h \
    $$PWD/print.h \
    $$PWD/selectclubdialog.h \
    $$PWD/selectcompetitiondialog.h \
    $$PWD/selectdetaildialog.h \
    $$PWD/selectdisciplinedialog.h \
    $$PWD/selectparticipantdialog.h \
    $$PWD/selectsubdivisiondialog.h

SOURCES += \
    $$PWD/exportwidget.cpp \
    $$PWD/maildialog.cpp \
    $$PWD/print.cpp \
    $$PWD/selectclubdialog.cpp \
    $$PWD/selectcompetitiondialog.cpp \
    $$PWD/selectdetaildialog.cpp \
    $$PWD/selectdisciplinedialog.cpp \
    $$PWD/selectparticipantdialog.cpp \
    $$PWD/selectsubdivisiondialog.cpp
