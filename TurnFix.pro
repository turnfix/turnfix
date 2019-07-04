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

include(participants/participants.pri)

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
    src/dialogs/others/header/dlg_barcode.h \
    view/dialogs/maildialog.h \
    src/dialogs/participants/header/dlg_pass.h \
    src/dialogs/points/header/dlg_bogen.h \
    view/dialogs/scorecarddialog.h \
    view/dialogs/selectdisciplinedialog.h \
    src/dialogs/select/header/dlg_select_rg.h \
    src/dialogs/select/header/dlg_select_tn.h \
    src/dialogs/select/header/dlg_select_wk.h \
    src/dialogs/wk/header/dlg_event.h \
    src/dialogs/wk/header/dlg_wk.h \
    src/global/header/_delegates.h \
    src/global/header/_global.h \
    src/misc/header/gri_editoritem.h \
    src/misc/header/trd_progress.h \
    src/models/header/mdl_karte.h \
    model/viewmodels/assignmenttablemodel.h \
    model/viewmodels/qualitablemodel.h \
    model/viewmodels/squadtablemodel.h \
    src/models/header/mdl_result.h \
    src/models/header/mdl_status.h \
    src/web/header/web_sendmail.h \
    src/widgets/header/wdg_tab_dr.h \
    src/widgets/header/wdg_tab_er.h \
    src/widgets/header/wdg_tab_pe.h \
    src/widgets/header/wdg_tab_rg.h \
    src/widgets/header/wdg_tab_st.h \
    src/widgets/header/wdg_tab_wk.h \
    view/windows/mainwindow.h \
    src/misc/header/gsc_editorscene.h \
    src/models/header/mdl_editor.h \
    src/dialogs/database/header/dlg_db_land.h \
    src/dialogs/database/header/dlg_db_verband.h \
    src/dialogs/database/header/dlg_db_gau.h \
    src/dialogs/database/header/dlg_db_penalty.h \
    src/widgets/header/wdg_tab_ed.h \
    export/print.h \
    export/misc/card.h \
    export/misc/certificate.h \
    export/misc/clublist.h \
    export/misc/invitation.h \
    export/misc/itchecksquads.h \
    export/misc/medalcount.h \
    export/misc/registrationmatrix.h \
    export/misc/summary.h \
    export/misc/timetable.h \
    export/participants/itchecklist.h \
    export/participants/judgessheet.h \
    export/participants/license.h \
    export/participants/list.h \
    export/participants/registration.h \
    export/participants/resultssheet.h \
    export/participants/squad.h \
    export/results/detail.h \
    export/results/individual.h \
    export/results/results.h \
    export/results/round.h \
    export/results/table.h \
    libs/fparser/fparser.hh \
    libs/fparser/fparser_gmpint.hh \
    libs/fparser/fparser_mpfr.hh \
    libs/fparser/fpconfig.hh \
    libs/fparser/extrasrc/fpaux.hh \
    libs/fparser/extrasrc/fptypes.hh \
    src/global/header/settings.h \
    src/dialogs/others/header/dlg_db_check_dialog.h \
    src/misc/header/trd_checktablesthread.h \
    src/dialogs/select/header/dlg_select_layout.h \
    src/dialogs/others/header/dlg_save_layout.h \
    src/dialogs/database/header/dlg_db_disgroup.h \
    src/dialogs/wk/header/dlg_delete.h \
    src/dialogs/select/header/dlg_select_detail.h \
    src/dialogs/database/header/dlg_db_formel.h \
    src/global/header/result_calc.h \
    src/widgets/others/header/wdg_pref.h \
    src/widgets/subclasses/header/extendedtabwidget.h \
    src/widgets/others/header/wdg_riegen.h \
    src/widgets/header/wdg_tab_sr.h \
    src/widgets/subclasses/header/extendedlistwidget.h \
    model/database/db.h \
    model/settings/tfsettings.h \
    view/dialogs/checkdatabasedialog.h \
    view/dialogs/logindialog.h \
    view/dialogs/selectclubdialog.h \
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
    export/print.cpp \
    export/misc/card.cpp \
    export/misc/certificate.cpp \
    export/misc/clublist.cpp \
    export/misc/invitation.cpp \
    export/misc/itchecksquads.cpp \
    export/misc/medalcount.cpp \
    export/misc/registrationmatrix.cpp \
    export/misc/summary.cpp \
    export/misc/timetable.cpp \
    export/participants/itchecklist.cpp \
    export/participants/judgessheet.cpp \
    export/participants/license.cpp \
    export/participants/list.cpp \
    export/participants/registration.cpp \
    export/participants/resultssheet.cpp \
    export/participants/squad.cpp \
    export/results/results.cpp \
    export/results/individual.cpp \
    export/results/detail.cpp \
    export/results/round.cpp \
    export/results/table.cpp \
    libs/fparser/fparser.cc \
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
    model/viewmodels/assignmenttablemodel.cpp \
    model/viewmodels/eventtablemodel.cpp \
    model/viewmodels/qualitablemodel.cpp \
    model/viewmodels/squadtablemodel.cpp \
    view/dialogs/checkdatabasedialog.cpp \
    view/dialogs/logindialog.cpp \
    view/dialogs/selectclubdialog.cpp \
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
    src/dialogs/others/dlg_barcode.cpp \
    view/dialogs/maildialog.cpp \
    src/dialogs/participants/dlg_pass.cpp \
    src/dialogs/points/dlg_bogen.cpp \
    view/dialogs/scorecarddialog.cpp \
    view/dialogs/selectdisciplinedialog.cpp \
    src/dialogs/select/dlg_select_rg.cpp \
    src/dialogs/select/dlg_select_tn.cpp \
    src/dialogs/select/dlg_select_wk.cpp \
    src/dialogs/wk/dlg_event.cpp \
    src/dialogs/wk/dlg_wk.cpp \
    src/global/_delegates.cpp \
    src/global/_global.cpp \
    src/misc/gri_editoritem.cpp \
    src/misc/trd_progress.cpp \
    src/models/mdl_karte.cpp \
    src/models/mdl_result.cpp \
    src/models/mdl_status.cpp \
    src/web/web_sendmail.cpp \
    src/widgets/wdg_tab_dr.cpp \
    src/widgets/wdg_tab_er.cpp \
    src/widgets/wdg_tab_pe.cpp \
    src/widgets/wdg_tab_rg.cpp \
    src/widgets/wdg_tab_st.cpp \
    src/widgets/wdg_tab_wk.cpp \
    src/misc/gsc_editorscene.cpp \
    src/models/mdl_editor.cpp \
    src/dialogs/database/dlg_db_land.cpp \
    src/dialogs/database/dlb_db_verband.cpp \
    src/dialogs/database/dlg_db_gau.cpp \
    src/dialogs/database/dlg_db_penalty.cpp \
    src/widgets/wdg_tab_ed.cpp \
    src/global/settings.cpp \
    src/dialogs/others/dlg_db_check_dialog.cpp \
    src/misc/trd_checktablesthread.cpp \
    src/dialogs/select/dlg_select_layout.cpp \
    src/dialogs/others/dlg_save_layout.cpp \
    src/dialogs/database/dlg_db_disgroup.cpp \
    src/dialogs/wk/dlg_delete.cpp \
    src/dialogs/select/dlg_select_detail.cpp \
    src/dialogs/database/dlg_db_formel.cpp \
    src/global/result_calc.cpp \
    src/widgets/others/wdg_pref.cpp \
    src/widgets/subclasses/extendedtabwidget.cpp \
    src/widgets/others/wdg_riegen.cpp \
    src/widgets/wdg_tab_sr.cpp \
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
    src/dialogs/others/ui/dlg_barcode.ui \
    view/dialogs/maildialog.ui \
    src/dialogs/participants/ui/dlg_pass.ui \
    src/dialogs/points/ui/dlg_bogen.ui \
    src/dialogs/points/ui/dlg_jury.ui \
    view/dialogs/scorecarddialog.ui \
    view/dialogs/selectclubdialog.ui \
    view/dialogs/selectdisciplinedialog.ui \
    src/dialogs/select/ui/dlg_select_tn.ui \
    src/dialogs/select/ui/dlg_select_rg.ui \
    view/windows/mainwindow.ui \
    view/widgets/settingswidget.ui \
    view/dialogs/logindialog.ui \
    view/dialogs/checkdatabasedialog.ui \
    view/wizards/postgressetupwizard.ui \
    src/dialogs/select/ui/dlg_select_wk.ui \
    src/dialogs/wk/ui/dlg_event.ui \
    src/dialogs/wk/ui/dlg_wk.ui \
    src/widgets/ui/wdg_tab_dr.ui \
    src/widgets/ui/wdg_tab_er.ui \
    src/widgets/ui/wdg_tab_pe.ui \
    src/widgets/ui/wdg_tab_rg.ui \
    src/widgets/ui/wdg_tab_st.ui \
    src/widgets/ui/wdg_tab_wk.ui \
    src/dialogs/database/ui/dlg_db_land.ui \
    src/dialogs/database/ui/dlg_db_verband.ui \
    src/dialogs/database/ui/dlg_db_gau.ui \
    src/dialogs/database/ui/dlg_db_penalty.ui \
    src/widgets/ui/wdg_tab_ed.ui \
    src/dialogs/others/ui/dlg_db_check_dialog.ui \
    src/dialogs/select/ui/dlg_select_layout.ui \
    src/dialogs/others/ui/dlg_save_layout.ui \
    src/dialogs/database/ui/dlg_db_disgroup.ui \
    src/dialogs/wk/ui/dlg_delete.ui \
    src/dialogs/select/ui/dlg_select_detail.ui \
    src/dialogs/database/ui/dlg_db_formel.ui \
    src/widgets/others/ui/wdg_pref.ui \
    src/widgets/others/ui/wdg_riegen.ui \
    src/widgets/ui/wdg_tab_sr.ui
RESOURCES += resources/TurnFix.qrc \
    resources/TF_Language.qrc \
    resources/TF_GeraeteIcons.qrc \
    assets/icons/icons.qrc
include(libs/qtpropertybrowser/src/qtpropertybrowser.pri)

DISTFILES += \
    .gitignore
