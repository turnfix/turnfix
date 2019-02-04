TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
QT += xml sql network
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
    LIBS += "-L/Library/PostgreSQL/8.4/lib" -lpq
}
win32 {
    TARGET = TurnFix
}

HEADERS += src/dialogs/database/header/dlg_db_acc.h \
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
    src/dialogs/others/header/dlg_mail.h \
    src/dialogs/participants/header/dlg_group.h \
    src/dialogs/participants/header/dlg_pass.h \
    src/dialogs/participants/header/dlg_team.h \
    src/dialogs/participants/header/dlg_tn.h \
    src/dialogs/points/header/dlg_bogen.h \
    src/dialogs/points/header/dlg_karte.h \
    src/dialogs/points/header/dlg_quali.h \
    src/dialogs/select/header/dlg_select_club.h \
    src/dialogs/select/header/dlg_select_dis.h \
    src/dialogs/select/header/dlg_select_rg.h \
    src/dialogs/select/header/dlg_select_tn.h \
    src/dialogs/select/header/dlg_select_wk.h \
    src/dialogs/wk/header/dlg_event.h \
    src/dialogs/wk/header/dlg_wk.h \
    src/global/header/_delegates.h \
    src/global/header/_global.h \
    src/misc/header/gri_editoritem.h \
    src/misc/header/trd_progress.h \
    src/models/header/mdl_einteilen.h \
    src/models/header/mdl_karte.h \
    src/models/header/mdl_quali.h \
    src/models/header/mdl_result.h \
    src/models/header/mdl_riege.h \
    src/models/header/mdl_status.h \
    src/web/header/web_sendmail.h \
    src/web/header/web_upload.h \
    src/widgets/header/wdg_tab_dr.h \
    src/widgets/header/wdg_tab_er.h \
    src/widgets/header/wdg_tab_pe.h \
    src/widgets/header/wdg_tab_rg.h \
    src/widgets/header/wdg_tab_st.h \
    src/widgets/header/wdg_tab_tn.h \
    src/widgets/header/wdg_tab_wk.h \
    src/windows/header/mainwindow.h \
    src/misc/header/gsc_editorscene.h \
    src/models/header/mdl_editor.h \
    src/dialogs/database/header/dlg_db_land.h \
    src/dialogs/database/header/dlg_db_verband.h \
    src/dialogs/database/header/dlg_db_gau.h \
    src/dialogs/database/header/dlg_db_penalty.h \
    src/export/header/drucken.h \
    src/widgets/header/wdg_tab_ed.h \
    src/export/ergebnisse/header/ergebnisse.h \
    src/export/ergebnisse/header/einzel.h \
    src/export/ergebnisse/header/detail.h \
    src/export/ergebnisse/header/runde.h \
    src/export/teilnehmerlisten/header/riege.h \
    src/export/teilnehmerlisten/header/liste.h \
    src/export/teilnehmerlisten/header/meldung.h \
    src/export/teilnehmerlisten/header/bogen.h \
    src/export/misc/header/karte.h \
    src/export/teilnehmerlisten/header/pass.h \
    src/export/teilnehmerlisten/header/edvchecktn.h \
    src/export/misc/header/urkunde.h \
    src/export/misc/header/meldematrix.h \
    src/export/misc/header/vereinsliste.h \
    src/export/misc/header/ausschreibung.h \
    src/export/misc/header/bericht.h \
    src/export/misc/header/zeitplan.h \
    src/export/misc/header/edvcheckriege.h \
    src/export/ergebnisse/header/tabelle.h \
    src/global/header/settings.h \
    src/dialogs/others/header/dlg_db_check_dialog.h \
    src/misc/header/trd_checktablesthread.h \
    src/dialogs/select/header/dlg_select_layout.h \
    src/dialogs/others/header/dlg_save_layout.h \
    src/dialogs/database/header/dlg_db_disgroup.h \
    src/dialogs/wk/header/dlg_delete.h \
    src/dialogs/participants/header/dlg_club.h \
    src/dialogs/select/header/dlg_select_detail.h \
    src/libs/fparser/fptypes.hh \
    src/libs/fparser/fpconfig.hh \
    src/libs/fparser/fpaux.hh \
    src/libs/fparser/fparser.hh \
    src/dialogs/web/header/dlg_upload.h \
    src/export/misc/header/medallienspiegel.h \
    src/dialogs/database/header/dlg_db_formel.h \
    src/global/header/result_calc.h \
    src/widgets/others/header/wdg_pref.h \
    src/widgets/subclasses/header/extendedtabwidget.h \
    src/widgets/others/header/wdg_riegen.h \
    src/export/teilnehmerlisten/header/ergebnisbogen.h \
    src/widgets/header/wdg_tab_sr.h \
    src/widgets/subclasses/header/extendedlistwidget.h \
    model/database/db.h \
    model/settings/tfsettings.h \
    view/widgets/settingswidget.h \
    view/widgets/tftoolbarwidget.h \
    view/dialogs/logindialog.h \
    view/widgets/tfselectionbar.h \
    view/widgets/tfabstracttoolbar.h \
    model/viewmodels/eventtablemodel.h \
    model/objects/event.h \
    model/database/dbcolumn.h \
    model/database/dbtable.h \
    view/dialogs/checkdatabasedialog.h \
    view/widgets/tfheaderwidget.h \
    model/database/tfsqlquery.h \
    model/database/dbconstraint.h \
    model/database/dbchecker.h \
    model/objects/location.h \
    model/objects/tfobject.h \
    model/settings/session.h \
    view/wizards/postgressetupwizard.h \
    model/database/dbinitializerpostgres.h
SOURCES += src/main.cpp \
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
    src/dialogs/others/dlg_mail.cpp \
    src/dialogs/participants/dlg_group.cpp \
    src/dialogs/participants/dlg_pass.cpp \
    src/dialogs/participants/dlg_team.cpp \
    src/dialogs/participants/dlg_tn.cpp \
    src/dialogs/points/dlg_bogen.cpp \
    src/dialogs/points/dlg_karte.cpp \
    src/dialogs/points/dlg_quali.cpp \
    src/dialogs/select/dlg_select_club.cpp \
    src/dialogs/select/dlg_select_dis.cpp \
    src/dialogs/select/dlg_select_rg.cpp \
    src/dialogs/select/dlg_select_tn.cpp \
    src/dialogs/select/dlg_select_wk.cpp \
    src/dialogs/wk/dlg_event.cpp \
    src/dialogs/wk/dlg_wk.cpp \
    src/global/_delegates.cpp \
    src/global/_global.cpp \
    src/misc/gri_editoritem.cpp \
    src/misc/trd_progress.cpp \
    src/models/mdl_einteilen.cpp \
    src/models/mdl_karte.cpp \
    src/models/mdl_quali.cpp \
    src/models/mdl_result.cpp \
    src/models/mdl_riege.cpp \
    src/models/mdl_status.cpp \
    src/web/web_sendmail.cpp \
    src/web/web_upload.cpp \
    src/widgets/wdg_tab_dr.cpp \
    src/widgets/wdg_tab_er.cpp \
    src/widgets/wdg_tab_pe.cpp \
    src/widgets/wdg_tab_rg.cpp \
    src/widgets/wdg_tab_st.cpp \
    src/widgets/wdg_tab_tn.cpp \
    src/widgets/wdg_tab_wk.cpp \
    src/windows/mainwindow.cpp \
    src/misc/gsc_editorscene.cpp \
    src/models/mdl_editor.cpp \
    src/dialogs/database/dlg_db_land.cpp \
    src/dialogs/database/dlb_db_verband.cpp \
    src/dialogs/database/dlg_db_gau.cpp \
    src/dialogs/database/dlg_db_penalty.cpp \
    src/export/drucken.cpp \
    src/widgets/wdg_tab_ed.cpp \
    src/export/ergebnisse/ergebnisse.cpp \
    src/export/ergebnisse/einzel.cpp \
    src/export/ergebnisse/detail.cpp \
    src/export/ergebnisse/runde.cpp \
    src/export/teilnehmerlisten/riege.cpp \
    src/export/teilnehmerlisten/liste.cpp \
    src/export/teilnehmerlisten/meldung.cpp \
    src/export/teilnehmerlisten/bogen.cpp \
    src/export/misc/karte.cpp \
    src/export/teilnehmerlisten/pass.cpp \
    src/export/teilnehmerlisten/edvchecktn.cpp \
    src/export/misc/urkunde.cpp \
    src/export/misc/meldematrix.cpp \
    src/export/misc/vereinsliste.cpp \
    src/export/misc/ausschreibung.cpp \
    src/export/misc/bericht.cpp \
    src/export/misc/zeitplan.cpp \
    src/export/misc/edvcheckriege.cpp \
    src/export/ergebnisse/tabelle.cpp \
    src/global/settings.cpp \
    src/dialogs/others/dlg_db_check_dialog.cpp \
    src/misc/trd_checktablesthread.cpp \
    src/dialogs/select/dlg_select_layout.cpp \
    src/dialogs/others/dlg_save_layout.cpp \
    src/dialogs/database/dlg_db_disgroup.cpp \
    src/dialogs/wk/dlg_delete.cpp \
    src/dialogs/participants/dlg_club.cpp \
    src/dialogs/select/dlg_select_detail.cpp \
    src/libs/fparser/fparser.cc \
    src/dialogs/web/dlg_upload.cpp \
    src/export/misc/medallienspiegel.cpp \
    src/dialogs/database/dlg_db_formel.cpp \
    src/global/result_calc.cpp \
    src/widgets/others/wdg_pref.cpp \
    src/widgets/subclasses/extendedtabwidget.cpp \
    src/widgets/others/wdg_riegen.cpp \
    src/export/teilnehmerlisten/ergebnisbogen.cpp \
    src/widgets/wdg_tab_sr.cpp \
    src/widgets/subclasses/extendedlistwidget.cpp \
    model/database/db.cpp \
    model/settings/tfsettings.cpp \
    view/widgets/settingswidget.cpp \
    view/widgets/tftoolbarwidget.cpp \
    view/dialogs/logindialog.cpp \
    view/widgets/tfselectionbar.cpp \
    view/widgets/tfabstracttoolbar.cpp \
    model/viewmodels/eventtablemodel.cpp \
    model/objects/event.cpp \
    model/database/dbcolumn.cpp \
    model/database/dbtable.cpp \
    view/dialogs/checkdatabasedialog.cpp \
    view/widgets/tfheaderwidget.cpp \
    model/database/tfsqlquery.cpp \
    model/database/dbconstraint.cpp \
    model/database/dbchecker.cpp \
    model/objects/location.cpp \
    model/objects/tfobject.cpp \
    model/settings/session.cpp \
    view/wizards/postgressetupwizard.cpp \
    model/database/dbinitializerpostgres.cpp
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
    src/dialogs/others/ui/dlg_mail.ui \
    src/dialogs/participants/ui/dlg_group.ui \
    src/dialogs/participants/ui/dlg_pass.ui \
    src/dialogs/participants/ui/dlg_team.ui \
    src/dialogs/participants/ui/dlg_tn.ui \
    src/dialogs/points/ui/dlg_bogen.ui \
    src/dialogs/points/ui/dlg_jury.ui \
    src/dialogs/points/ui/dlg_karte.ui \
    src/dialogs/points/ui/dlg_quali.ui \
    src/dialogs/select/ui/dlg_select_club.ui \
    src/dialogs/select/ui/dlg_select_dis.ui \
    src/dialogs/select/ui/dlg_select_rg.ui \
    src/dialogs/select/ui/dlg_select_tn.ui \
    src/dialogs/select/ui/dlg_select_wk.ui \
    src/dialogs/wk/ui/dlg_event.ui \
    src/dialogs/wk/ui/dlg_wk.ui \
    src/widgets/ui/wdg_tab_dr.ui \
    src/widgets/ui/wdg_tab_er.ui \
    src/widgets/ui/wdg_tab_pe.ui \
    src/widgets/ui/wdg_tab_rg.ui \
    src/widgets/ui/wdg_tab_st.ui \
    src/widgets/ui/wdg_tab_tn.ui \
    src/widgets/ui/wdg_tab_wk.ui \
    src/windows/ui/mainwindow.ui \
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
    src/dialogs/participants/ui/dlg_club.ui \
    src/dialogs/select/ui/dlg_select_detail.ui \
    src/dialogs/web/ui/dlg_upload.ui \
    src/dialogs/database/ui/dlg_db_formel.ui \
    src/widgets/others/ui/wdg_pref.ui \
    src/widgets/others/ui/wdg_riegen.ui \
    src/widgets/ui/wdg_tab_sr.ui \
    view/widgets/settingswidget.ui \
    view/dialogs/logindialog.ui \
    view/dialogs/checkdatabasedialog.ui \
    view/wizards/postgressetupwizard.ui
RESOURCES += resources/TurnFix.qrc \
    resources/TF_Language.qrc \
    resources/TF_GeraeteIcons.qrc \
    assets/icons/icons.qrc
include(libs/qtpropertybrowser/src/qtpropertybrowser.pri)
