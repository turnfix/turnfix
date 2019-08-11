#include "header/_delegates.h"
#include "header/_global.h"
#include "model/entity/event.h"
#include <QApplication>
#include <QComboBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPainter>
#include <QSqlQuery>

QWidget *EditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const {
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void EditorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void EditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();
    model->setData(index, value, Qt::EditRole);
}

void EditorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const{
    editor->setGeometry(option.rect);
}

bool EditorDelegate::eventFilter ( QObject *object, QEvent *event) {
    QWidget *editor = qobject_cast<QWidget*>(object);
    if (!editor) return false;
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
            QMetaObject::invokeMethod(this, "_q_commitDataAndCloseEditor", Qt::QueuedConnection, Q_ARG(QWidget*, editor));
            return false;
        } else if (e->key() == Qt::Key_Tab) {
            QMetaObject::invokeMethod(this, "_q_commitDataAndCloseEditor", Qt::QueuedConnection, Q_ARG(QWidget*, editor));
            return false;
        } else if (e->key() == Qt::Key_Escape) {
            emit closeEditor(editor, QAbstractItemDelegate::RevertModelCache);
        }
    }
    return false;
}

CmbDelegate::CmbDelegate(Event *event, QObject *parent) : QItemDelegate(parent)
{
    this->m_event = event;
}

QWidget *CmbDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const {
    QComboBox *editor = new QComboBox(parent);
    QStringList fields;
    fields << "";
    //fields << "Pause";
    QSqlQuery query;
    query.prepare("SELECT var_kurz2 FROM tfx_wertungen INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid INNER JOIN tfx_wettkaempfe_x_disziplinen ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_veranstaltungenid=? AND tfx_wertungen.int_runde=? AND var_riege=? GROUP BY int_disziplinenid, var_kurz2 ORDER BY int_disziplinenid");
    query.bindValue(0, this->m_event->mainEvent()->id());
    query.bindValue(1, this->m_event->round());
    query.bindValue(2, index.model()->index(index.row(),0).data().toString());
    query.exec();
    while (query.next()) {
        fields << query.value(0).toString();
    }
    editor->addItems(fields);
    return editor;
}

void CmbDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(comboBox->findText(value));
}

void CmbDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    QSqlQuery query;
    query.prepare("SELECT int_riegen_x_disziplinenid FROM tfx_riegen_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_veranstaltungenid=? AND int_runde=? AND var_name=? AND var_riege=?");
    query.bindValue(0, this->m_event->mainEvent()->id());
    query.bindValue(1, this->m_event->round());
    query.bindValue(2, value);
    query.bindValue(3, index.model()->index(index.row(),0).data().toString());
    query.exec();
    if (_global::querySize(query)==0) {
        model->setData(index, value, Qt::EditRole);
        return;
    }
    query.next();
    QSqlQuery query2;
    query2.prepare("UPDATE tfx_riegen_x_disziplinen SET bol_erstes_geraet='true' WHERE int_riegen_x_disziplinenid = ?");
    query2.bindValue(0,query.value(0).toInt());
    query2.exec();
    query2.prepare("UPDATE tfx_riegen_x_disziplinen SET bol_erstes_geraet='false' WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege=? AND int_riegen_x_disziplinenid != ?");
    query2.bindValue(0, this->m_event->mainEvent()->id());
    query2.bindValue(1, this->m_event->round());
    query2.bindValue(2, index.model()->index(index.row(),0).data().toString());
    query2.bindValue(3, query.value(0).toInt());
    query2.exec();
    model->setData(index, value, Qt::EditRole);
}

void CmbDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const{
    editor->setGeometry(option.rect);
}

void AlignItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    QStyleOptionViewItem o = option;
    o.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
    QItemDelegate::paint( painter, o, index );
}

void AlignCItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    QStyleOptionViewItem o = option;
    o.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
    QItemDelegate::paint( painter, o, index );
}
