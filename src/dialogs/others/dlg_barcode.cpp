#include <QSqlQuery>
#include <QKeyEvent>
#include "header/dlg_barcode.h"

Barcode_Dialog::Barcode_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    txt_code->installEventFilter(this);
}

QString Barcode_Dialog::getInput() {
    return str;
}

bool Barcode_Dialog::eventFilter(QObject *obj, QEvent *event) {
    if (obj == txt_code) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *e = static_cast<QKeyEvent*>(event);
            if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
                str = txt_code->text();
                done(1);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
