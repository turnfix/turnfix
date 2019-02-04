#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QDesktopWidget>
#include <QSettings>
#include <QToolButton>
#include <QTimer>
#include <math.h>
#include <QInputDialog>
#include "header/win_eingabe.h"
#include "../dialogs/others/header/dlg_tool_login.h"
#include "../dialogs/select/header/dlg_select_dis.h"
#include "../dialogs/select/header/dlg_select_rg.h"
#include "header/win_display.h"
#include "../models/header/mdl_riege.h"
#include "../global/header/_delegates.h"
#include "../global/header/_global.h"
#include "../global/header/result_calc.h"
#include "../libs/fparser/fparser.hh"

Eingabe_Tool::Eingabe_Tool() {
    init();
    availRiegen.clear();
    Select_Rg_Dialog *rg = new Select_Rg_Dialog();
    if (rg->exec()==1) {
        availRiegen = rg->getRg();
    }
    availDisziplinen.clear();
    Select_Dis_Dialog *diszi = new Select_Dis_Dialog();
    if (diszi->exec() == 1) {
        for (int i=0;i<diszi->getDis().size();i++) {
            availDisziplinen.append(diszi->getDis().at(i).at(0));
        }
    }
    setupUi(this);
    showMaximized();
    dis = new Display_Window();
    bg = new QButtonGroup();
    bg2 = new QButtonGroup();
    page_2->setLayout(new FlowLayout);
    page->setLayout(new FlowLayout);
    red();
    resetClock();
    time = new QTimer(this);
    dis->lbl_time_2->setVisible(false);
    connect(but_start, SIGNAL(clicked()), this, SLOT(startClock()));
    connect(but_stop, SIGNAL(clicked()), this, SLOT(stopClock()));
    connect(but_reset, SIGNAL(clicked()), this, SLOT(resetClock()));
    connect(but_green, SIGNAL(clicked()), this, SLOT(green()));
    connect(but_red, SIGNAL(clicked()), this, SLOT(red()));
    connect(but_clockoff, SIGNAL(clicked()), this, SLOT(hideClock()));
    connect(dis, SIGNAL(clock(QString)), this, SLOT(updateClock(QString)));
    pe_model = new QRiegenTableModel();
    fillRiege();
    pe_table->setModel(pe_model);
    cotestantChange();
    connect(act_display, SIGNAL(triggered()), this, SLOT(showHideDisplay()));
    connect(dis, SIGNAL(finished(int)), act_display, SLOT(trigger()));
    connect(but_fw3, SIGNAL(clicked()), this, SLOT(fillTable()));
    connect(but_cancel, SIGNAL(clicked()), this, SLOT(back1()));
    connect(but_end, SIGNAL(clicked()), this, SLOT(end()));
    connect(bg,SIGNAL(buttonClicked(int)),this,SLOT(fillOrder()));
    connect(bg,SIGNAL(buttonClicked(int)), this, SLOT(updateDis()));
    connect(bg2,SIGNAL(buttonClicked(int)),this,SLOT(fillDis()));
    connect(pe_table->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(cotestantChange()));
    connect(cmb_club, SIGNAL(currentIndexChanged(int)), this, SLOT(etClubChange(int)));
    connect(tim_time, SIGNAL(timeChanged(QTime)), this, SLOT(etUpdateTime(QTime)));
    connect(time, SIGNAL(timeout()), this, SLOT(etSetTime()));
    connect(but_estart, SIGNAL(clicked()), this, SLOT(etStartStop()));
    connect(but_overview, SIGNAL(toggled(bool)), this, SLOT(showStanding(bool)));
}

void Eingabe_Tool::init() {
    QSqlQuery query;
    query.prepare("SELECT int_runde, int_hauptwettkampf FROM tfx_veranstaltungen WHERE int_veranstaltungenid=? LIMIT 1");
    query.bindValue(0,_global::getWkNr());
    query.exec();
    query.next();
    _global::setWkVars(_global::getWkNr(), query.value(0).toInt(), query.value(1).toInt());
}

void Eingabe_Tool::fillRiege() {
    foreach(QAbstractButton *button, bg2->buttons()) {
        page->layout()->removeWidget(button);
        bg2->removeButton(button);
        button->~QAbstractButton();
    }
    QString riegen;
    for (int i=0;i<availRiegen.size();i++) {
        riegen += "'"+availRiegen.at(i)+"'";
        if (i<availRiegen.size()-1) {
            riegen += ",";
        }
    }
    QSqlQuery query2;
    query2.prepare("SELECT DISTINCT(var_riege) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege IN ("+riegen+") ORDER BY var_riege");
    query2.bindValue(0,_global::checkHWK());
    query2.bindValue(1,_global::getRunde());
    query2.exec();
    while (query2.next()) {
        QToolButton *but = new QToolButton();
        but->setText(query2.value(0).toString());
        but->setCheckable(true);
        but->setFont(QFont("Arial",36));
        but->setFixedSize(135,120);
        page->layout()->addWidget(but);
        bg2->addButton(but);
    }
    stackedWidget->setCurrentIndex(0);
}

void Eingabe_Tool::fillDis() {
    foreach(QAbstractButton *button, bg->buttons()) {
        page_2->layout()->removeWidget(button);
        bg->removeButton(button);
        button->~QAbstractButton();
    }
    QSqlQuery query2;
    query2.prepare("SELECT DISTINCT int_disziplinenid, tfx_disziplinen.var_name, var_icon, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wertungen USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_riege=? AND int_disziplinenid IN ("+_global::intListToString(availDisziplinen)+") ORDER BY tfx_disziplinen.var_name, kp");
    query2.bindValue(0,_global::checkHWK());
    query2.bindValue(1,bg2->checkedButton()->text());
    query2.exec();
    if (query2.size() == 0) {
        QMessageBox msg(QMessageBox::Warning,"Riege nicht vorhanden!","Die von Ihnen gewählte Riege existiert nicht!");
        msg.exec();
    } else {
        while (query2.next()) {
            bool kp=false;
            if (query2.value(3).toInt() == 1) kp=true;
            for (int i=0;i<=query2.value(3).toInt();i++) {
                QString text = query2.value(1).toString();
                if (kp == true) {
                    if (i == 0) {
                        text += " (Pflicht)";
                    } else {
                        text += " (Kür)";
                    }
                }
                QToolButton *but = new QToolButton();
                but->setIcon(QIcon(query2.value(2).toString()));
                but->setText(text);
                but->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                but->setCheckable(true);
                but->setProperty("id",query2.value(0).toInt());
                but->setProperty("file",query2.value(2).toString());
                but->setIconSize(QSize(120,120));
                but->setFont(QFont("Arial",20));
                but->setFixedSize(135,120);
                page_2->layout()->addWidget(but);
                bg->addButton(but);
            }
        }
        stackedWidget->setCurrentIndex(1);
    }
}

void Eingabe_Tool::fillOrder() {
    int disid = bg->checkedButton()->property("id").toInt();
    if (disid>0) {
        listWidget->init();
        listWidget->setRgDis(bg2->checkedButton()->text(),bg->checkedButton()->text());
        QStringList clubs = listWidget->getClubs();
        cmb_club->clear();
        cmb_club->addItem("Verein wählen");
        cmb_club->addItems(clubs);
        stackedWidget->setCurrentIndex(2);
    } else {
        QMessageBox msg(QMessageBox::Warning,"Keine Disziplin gewählt!","Bitte wählen Sie eine Disziplin!");
        msg.exec();
    }
}

void Eingabe_Tool::fillTable() {
    pe_table->clearSelection();
    int disid = bg->checkedButton()->property("id").toInt();
    bool kuer=false;
    if (bg->checkedButton()->text().right(5) == "(Kür)") kuer = true;
    QSqlQuery getVersuche;
    getVersuche.prepare("SELECT int_versuche FROM tfx_disziplinen WHERE int_disziplinenid=?");
    getVersuche.bindValue(0,disid);
    getVersuche.exec();
    getVersuche.next();
    pe_table->clearSpans();
    pe_model->setTableData(bg2->checkedButton()->text(),disid,getVersuche.value(0).toInt(),kuer,true);
    QHeaderView::ResizeMode resizeMode[pe_model->columnCount()];
    resizeMode[0] = QHeaderView::ResizeToContents;
    resizeMode[1] = QHeaderView::Stretch;
    resizeMode[2] = QHeaderView::Stretch;
    resizeMode[3] = QHeaderView::ResizeToContents;
    int resize[pe_model->columnCount()];
    resize[0] = 40;
    resize[1] = 200;
    resize[2] = 200;
    resize[3] = 35;
    int k=4;
    for (int i=k;i<pe_model->columnCount();i++) {
        resizeMode[k] = QHeaderView::ResizeToContents;
        resize[k] = 60;
        k++;
    }
    for (int i=0;i<pe_model->columnCount();i++) {
        pe_table->horizontalHeader()->setResizeMode(i, resizeMode[i]);
        pe_table->horizontalHeader()->resizeSection(i, resize[i]);
        if (i > 3) {
            editorDelegate *ed = new editorDelegate;
            connect(ed, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(finishEdit()));
            pe_table->setItemDelegateForColumn(i,ed);
        } else if (i==3) {
            pe_table->setItemDelegateForColumn(i,new alignCItemDelegate);
        }
    }
    if (getVersuche.value(0).toInt()>1) {
        for (int i=0;i<pe_model->rowCount();i++) {
            if (i%getVersuche.value(0).toInt() == 0) {
                pe_table->setSpan(i,0,getVersuche.value(0).toInt(),1);
                pe_table->setSpan(i,1,getVersuche.value(0).toInt(),1);
                pe_table->setSpan(i,2,getVersuche.value(0).toInt(),1);
                pe_table->setSpan(i,3,getVersuche.value(0).toInt(),1);
            }
        }
    }
    pe_table->setCurrentIndex(pe_model->index(0,4));
    pe_table->edit(pe_table->currentIndex());

    stackedWidget->setCurrentIndex(3);
    dis->main->setCurrentIndex(1);
}

void Eingabe_Tool::back1() {
    stackedWidget->setCurrentIndex(0);
}
void Eingabe_Tool::end() {
    finishEdit();
    fillRiege();
    stackedWidget->setCurrentIndex(0);
    dis->main->setCurrentIndex(0);
}

void Eingabe_Tool::cotestantChange(bool isLast) {
    if (pe_table->currentIndex().isValid() && pe_model->rowCount() > 0) {
        int row = pe_table->currentIndex().row();
        if (!isLast) {
            QSqlQuery current;
            current.prepare("SELECT var_nachname, var_vorname, int_startnummer FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_wertungenid=?");
            current.bindValue(0,pe_model->getCurrentID(pe_table->currentIndex()));
            current.exec();
            current.next();
            dis->updateCurrent(current.value(0).toString(),current.value(1).toString(),QVariant(pe_model->data(pe_model->index(row,2))).toString(),current.value(2).toString());
        } else {
            dis->updateCurrent("","","","");
        }
        if (pe_model->getNextID(pe_table->currentIndex()) > 0) {
            QSqlQuery next;
            next.prepare("SELECT var_nachname, var_vorname FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_wertungenid=?");
            next.bindValue(0,pe_model->getNextID(pe_table->currentIndex()));
            next.exec();
            next.next();
            dis->updateNext("N: "+next.value(0).toString().toUpper()+" "+next.value(1).toString());
        } else {
            dis->updateNext("");
        }
        if (pe_model->getLastID(pe_table->currentIndex()) > 0) {
            QSqlQuery last;
            last.prepare("SELECT var_nachname, var_vorname FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_wertungenid=?");
            if (!isLast) {
                last.bindValue(0,pe_model->getLastID(pe_table->currentIndex()));
            } else {
                last.bindValue(0,pe_model->getCurrentID(pe_table->currentIndex()));
            }
            last.exec();
            last.next();
            dis->updateLast(last.value(0).toString().toUpper()+" "+last.value(1).toString(),getScore(isLast));
        } else {
            dis->updateLast("","");
        }
    }
    dis->hideClock();
    dis->updateAmpel(1);
}
void Eingabe_Tool::red() {
    dis->updateAmpel(1);
}
void Eingabe_Tool::green() {
    dis->updateAmpel(0);
}
void Eingabe_Tool::startClock() {
    dis->startClock();
}
void Eingabe_Tool::stopClock() {
    dis->stopClock();
}
void Eingabe_Tool::resetClock() {
    dis->resetClock();
}

void Eingabe_Tool::hideClock() {
    dis->hideClock();
}

void Eingabe_Tool::updateClock(QString str) {
    lbl_clock->setText(str);
}

void Eingabe_Tool::updateDis() {
    dis->updateDis(bg->checkedButton()->property("file").toString(),bg->checkedButton()->text());
}

void Eingabe_Tool::showHideDisplay() {
    if (act_display->isChecked()) {
        bool ok;
        int i = QInputDialog::getInt(this, tr("Bildschirm wählen"), tr("Bitte die Bildschirmnummer eingeben auf der das Display angezeigt werden soll."), 2, 1, 100, 1, &ok);
        if (ok) {
            i--;
            QDesktopWidget *dw = QApplication::desktop();
            dis->setFixedHeight(dw->screenGeometry(i).height());
            dis->resize();
            dis->show();
            dis->setGeometry(dw->screenGeometry(i));
        } else {
            act_display->setChecked(false);
            return;
        }
    } else {
        dis->hide();
    }
}

QString Eingabe_Tool::getScore(bool last) {
    int disid = bg->checkedButton()->property("id").toInt();
    QString wert;
    QString wert2;
    QSqlQuery query;
    query.prepare("SELECT rel_leistung FROM tfx_wertungen_details INNER JOIN tfx_wertungen USING (int_wertungenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_disziplinenid=? AND int_wertungenid=? AND int_runde=? ORDER BY rel_leistung DESC LIMIT 1");
    query.bindValue(0, _global::checkHWK());
    query.bindValue(1, disid);
    if (!last) {
        query.bindValue(2, pe_model->getLastID(pe_table->currentIndex()));
    } else {
        query.bindValue(2, pe_model->getCurrentID(pe_table->currentIndex()));
    }
    query.bindValue(3, _global::getRunde());
    query.exec();
    QSqlQuery query3;
    query3.prepare("SELECT rel_leistung FROM tfx_jury_results INNER JOIN tfx_wertungen USING (int_wertungenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_disziplinen_felder ON  tfx_disziplinen_felder.int_disziplinen_felderid = tfx_jury_results.int_disziplinen_felderid WHERE int_veranstaltungenid=? AND int_disziplinenid=? AND int_wertungenid=? AND int_runde=? AND bol_ausgangswert='true' ORDER BY rel_leistung DESC LIMIT 1");
    query3.bindValue(0, _global::checkHWK());
    query3.bindValue(1, disid);
    if (!last) {
        query3.bindValue(2, pe_model->getLastID(pe_table->currentIndex()));
    } else {
        query3.bindValue(2, pe_model->getCurrentID(pe_table->currentIndex()));
    }
    query3.bindValue(3, _global::getRunde());
    query3.exec();
    QSqlQuery query2;
    query2.prepare("SELECT int_disziplinenid, var_maske, var_einheit, int_berechnung FROM tfx_disziplinen WHERE int_disziplinenid=?");
    query2.bindValue(0,disid);
    query2.exec();
    query2.next();
    if (query.size() == 0) {
        wert = query2.value(1).toString();
    } else {
        query.next();
        if (query3.size() == 0) {
            wert2 = query2.value(1).toString();
        } else {
            query3.next();
            double aw = query3.value(0).toDouble();
            if (chk_plusten->isChecked()) aw += 10.0;
            wert2 = _global::strLeistung(aw,query2.value(2).toString(),"0.00",2);
        }
        wert = _global::strLeistung(query.value(0).toDouble(),query2.value(2).toString(),query2.value(1).toString(),query2.value(3).toInt()) + " (" + wert2 + ") ";
    }
    return wert;
}

void Eingabe_Tool::finishEdit() {
    int row = pe_table->currentIndex().row();
    int col = pe_table->currentIndex().column();
    if (col == pe_model->columnCount()-1) {
        if (pe_model->index(row+1,4).isValid()) {
            pe_table->setCurrentIndex(pe_model->index(row+1,4));
        } else {
            this->cotestantChange(true);
            //pe_table->setCurrentIndex(pe_model->index(0,4));
        }
    } else {
        pe_table->setCurrentIndex(pe_model->index(row,col+1));
        calc();
    }
}

void Eingabe_Tool::calc() {
    QStringList lst;
    lst << "A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K"<<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"<<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z";
    QSqlQuery query2;
    query2.prepare("SELECT tfx_formeln.var_formel, int_berechnung, var_einheit, var_maske FROM tfx_disziplinen LEFT JOIN tfx_formeln USING (int_formelid) WHERE int_disziplinenid=?");
    query2.bindValue(0,bg->checkedButton()->property("id").toInt());
    query2.exec();
    query2.next();
    QString vars;
    for (int i=0;i<lst.size();i++) {
        if (query2.value(0).toString().contains(lst.at(i))) {
            vars += lst.at(i)+",";
        }
    }
    vars = vars.left(vars.length()-1);
    FunctionParser fparser;
    fparser.Parse(query2.value(0).toString().toStdString(),vars.toStdString());
    int size = pe_model->columnCount()-5;
    double Vars[size];
    for (int i=4;i<pe_model->columnCount()-1;i++) {
        Vars[i-4] = QVariant(pe_model->data(pe_model->index(pe_table->currentIndex().row(),i))).toDouble();
    }
    double res = fparser.Eval(Vars);
    pe_model->setData(pe_model->index(pe_table->currentIndex().row(),pe_model->columnCount()-1),_global::strLeistung(res,query2.value(2).toString(),query2.value(3).toString(),query2.value(1).toInt()),Qt::EditRole);
}

void Eingabe_Tool::showStanding(bool showit) {
    if (showit) {
        if (pe_model->rowCount()>0) {
            QString wkNummer = QVariant(pe_model->data(pe_model->index(0,3))).toString();
            QList<QStringList> list = Result_Calc::resultArrayNew(wkNummer);
            int wktyp = _global::checkTyp(wkNummer);
            int hwk = _global::checkHWK();
            dis->er_model->setList(list,wkNummer,hwk,wktyp,false,false);
            if (list.size() > 0) {
                int size = list.at(0).size();
                QHeaderView::ResizeMode resizeModeER[size];
                resizeModeER[0] = QHeaderView::ResizeToContents;
                resizeModeER[1] = QHeaderView::Stretch;
                resizeModeER[2] = QHeaderView::ResizeToContents;
                resizeModeER[3] = QHeaderView::ResizeToContents;
                resizeModeER[4] = QHeaderView::Fixed;
                for (int i=0;i<size;i++) {
                    dis->tbl_overview->horizontalHeader()->setResizeMode(i, resizeModeER[i]);
                }
                dis->tbl_overview->horizontalHeader()->setSectionHidden(4,true);
            }
        }
        dis->main->setCurrentIndex(3);
    } else {
        dis->main->setCurrentIndex(1);
    }
}

void Eingabe_Tool::closeEvent(QCloseEvent*) {
    dis->close();
}

void Eingabe_Tool::etClubChange(int idx) {
    if (idx == 0) {
        dis->main->setCurrentIndex(0);
    } else {
        dis->lbl_verein->setText(cmb_club->currentText());
        dis->main->setCurrentIndex(2);
    }
}

void Eingabe_Tool::etUpdateTime(QTime time) {
    dis->lbl_time_2->setText(time.toString("m:ss"));
    if (dis->lbl_time_2->text() == "0:00") {
        dis->lbl_time_2->setVisible(false);
        dis->sta_ampel_2->setCurrentIndex(1);
        this->time->stop();
    } else {
        dis->lbl_time_2->setVisible(true);
    }
}

void Eingabe_Tool::etStartStop() {
    if (time->isActive()) {
        time->stop();
        dis->sta_ampel_2->setCurrentIndex(1);
    } else {
        time->start(1000);
        dis->sta_ampel_2->setCurrentIndex(0);
    }
}

void Eingabe_Tool::etSetTime() {
    tim_time->setTime(tim_time->time().addSecs(-1));
}

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

int FlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0) {
        return m_hSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int FlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0) {
        return m_vSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int FlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    else
        return 0;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return 0;
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    QLayoutItem *item;
    foreach (item, itemList)
        size = size.expandedTo(item->minimumSize());

    size += QSize(2*margin(), 2*margin());
    return size;
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    QLayoutItem *item;
    foreach (item, itemList) {
        QWidget *wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        int spaceY = verticalSpacing();
        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}
int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}
