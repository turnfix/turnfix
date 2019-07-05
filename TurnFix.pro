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

include(competitions/competitions.pri)
include(participants/participants.pri)
include(subdivisions/subdivisions.pri)
include(startingorder/startingorder.pri)
include(capture/capture.pri)
include(results/results.pri)
include(export/export.pri)
include(status/status.pri)
include(editor/editor.pri)

HEADERS += src/dialogs/database/header/dlg_db_acc.h \
    model/objects/competition.h \
    src/dialogs/database/header/dlg_db_club.h \
    src/dialogs/database/header/dlg_db_control.h \
    src/dialogs/database/header/dlg_db_dis.h \
    src/dialogs/database/header/dlg_db_loc.h \
    src/dialogs/database/header/dlg_db_person.h \
    src/dialogs/database/header/dlg_db_sec.h \
    src/dialogs/database/header/dlg_db_sport.h \
    src/dialogs/database/header/dlg_db_status.h \
    src/dialogs/database/header/dlg_db_tn.h \
    src/dialogs/others/header/dlg_about.h \
    src/dialogs/participants/header/dlg_pass.h \
    view/dialogs/scorecarddialog.h \
    src/dialogs/wk/header/dlg_event.h \
    src/global/header/_delegates.h \
    src/global/header/_global.h \
    src/misc/header/trd_progress.h \
    src/models/header/mdl_karte.h \
    model/viewmodels/qualitablemodel.h \
    src/web/header/web_sendmail.h \
    view/windows/mainwindow.h \
    src/models/header/mdl_editor.h \
    src/dialogs/database/header/dlg_db_land.h \
    src/dialogs/database/header/dlg_db_verband.h \
    src/dialogs/database/header/dlg_db_gau.h \
    src/dialogs/database/header/dlg_db_penalty.h \
    src/global/header/settings.h \
    src/dialogs/others/header/dlg_db_check_dialog.h \
    src/misc/header/trd_checktablesthread.h \
    src/dialogs/database/header/dlg_db_disgroup.h \
    src/dialogs/wk/header/dlg_delete.h \
    src/dialogs/database/header/dlg_db_formel.h \
    src/global/header/result_calc.h \
    src/widgets/others/header/wdg_pref.h \
    src/widgets/subclasses/header/extendedtabwidget.h \
    src/widgets/subclasses/header/extendedlistwidget.h \
    model/database/db.h \
    model/settings/tfsettings.h \
    view/dialogs/checkdatabasedialog.h \
    view/dialogs/logindialog.h \
    view/widgets/settingswidget.h \
    view/widgets/tftoolbarwidget.h \
    view/widgets/tfselectionbar.h \
    view/widgets/tfabstracttoolbar.h \
    view/widgets/tfheaderwidget.h \
    view/wizards/postgressetupwizard.h \
    model/viewmodels/eventtablemodel.h \
    model/objects/event.h \
    model/database/dbcolumn.h \
    model/database/dbtable.h \
    model/database/tfsqlquery.h \
    model/database/dbconstraint.h \
    model/database/dbchecker.h \
    model/objects/location.h \
    model/settings/session.h \
    model/database/dbinitializerpostgres.h
SOURCES += main.cpp \
    model/database/db.cpp \
    model/database/dbchecker.cpp \
    model/database/dbcolumn.cpp \
    model/database/dbconstraint.cpp \
    model/database/dbinitializerpostgres.cpp \
    model/database/dbtable.cpp \
    model/database/tfsqlquery.cpp \
    model/objects/competition.cpp \
    model/objects/event.cpp \
    model/objects/location.cpp \
    model/settings/session.cpp \
    model/settings/tfsettings.cpp \
    model/viewmodels/eventtablemodel.cpp \
    model/viewmodels/qualitablemodel.cpp \
    view/dialogs/checkdatabasedialog.cpp \
    view/dialogs/logindialog.cpp \
    view/widgets/settingswidget.cpp \
    view/widgets/tfabstracttoolbar.cpp \
    view/widgets/tfheaderwidget.cpp \
    view/widgets/tfselectionbar.cpp \
    view/widgets/tftoolbarwidget.cpp \
    view/windows/mainwindow.cpp \
    view/wizards/postgressetupwizard.cpp \
    src/dialogs/database/dlg_db_acc.cpp \
    src/dialogs/database/dlg_db_club.cpp \
    src/dialogs/database/dlg_db_control.cpp \
    src/dialogs/database/dlg_db_dis.cpp \
    src/dialogs/database/dlg_db_loc.cpp \
    src/dialogs/database/dlg_db_person.cpp \
    src/dialogs/database/dlg_db_sec.cpp \
    src/dialogs/database/dlg_db_sport.cpp \
    src/dialogs/database/dlg_db_status.cpp \
    src/dialogs/database/dlg_db_tn.cpp \
    src/dialogs/participants/dlg_pass.cpp \
    view/dialogs/scorecarddialog.cpp \
    src/dialogs/wk/dlg_event.cpp \
    src/global/_delegates.cpp \
    src/global/_global.cpp \
    src/misc/trd_progress.cpp \
    src/models/mdl_karte.cpp \
    src/web/web_sendmail.cpp \
    src/models/mdl_editor.cpp \
    src/dialogs/database/dlg_db_land.cpp \
    src/dialogs/database/dlb_db_verband.cpp \
    src/dialogs/database/dlg_db_gau.cpp \
    src/dialogs/database/dlg_db_penalty.cpp \
    src/global/settings.cpp \
    src/dialogs/others/dlg_db_check_dialog.cpp \
    src/misc/trd_checktablesthread.cpp \
    src/dialogs/database/dlg_db_disgroup.cpp \
    src/dialogs/wk/dlg_delete.cpp \
    src/dialogs/database/dlg_db_formel.cpp \
    src/global/result_calc.cpp \
    src/widgets/others/wdg_pref.cpp \
    src/widgets/subclasses/extendedtabwidget.cpp \
    src/widgets/subclasses/extendedlistwidget.cpp
FORMS += src/dialogs/database/ui/dlg_db_acc.ui \
    src/dialogs/database/ui/dlg_db_club.ui \
    src/dialogs/database/ui/dlg_db_control.ui \
    src/dialogs/database/ui/dlg_db_dis.ui \
    src/dialogs/database/ui/dlg_db_loc.ui \
    src/dialogs/database/ui/dlg_db_person.ui \
    src/dialogs/database/ui/dlg_db_sec.ui \
    src/dialogs/database/ui/dlg_db_sport.ui \
    src/dialogs/database/ui/dlg_db_status.ui \
    src/dialogs/database/ui/dlg_db_tn.ui \
    src/dialogs/others/ui/dlg_about.ui \
    src/dialogs/participants/ui/dlg_pass.ui \
    src/dialogs/points/ui/dlg_jury.ui \
    view/dialogs/scorecarddialog.ui \
    view/windows/mainwindow.ui \
    view/widgets/settingswidget.ui \
    view/dialogs/logindialog.ui \
    view/dialogs/checkdatabasedialog.ui \
    view/wizards/postgressetupwizard.ui \
    src/dialogs/wk/ui/dlg_event.ui \
    src/dialogs/database/ui/dlg_db_land.ui \
    src/dialogs/database/ui/dlg_db_verband.ui \
    src/dialogs/database/ui/dlg_db_gau.ui \
    src/dialogs/database/ui/dlg_db_penalty.ui \
    src/dialogs/others/ui/dlg_db_check_dialog.ui \
    src/dialogs/database/ui/dlg_db_disgroup.ui \
    src/dialogs/wk/ui/dlg_delete.ui \
    src/dialogs/database/ui/dlg_db_formel.ui \
    src/widgets/others/ui/wdg_pref.ui
RESOURCES += resources/TurnFix.qrc \
    resources/TF_Language.qrc \
    resources/TF_GeraeteIcons.qrc \
    assets/icons/icons.qrc
include(libs/libs.pri)

DISTFILES += \
    .gitignore
