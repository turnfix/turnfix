#include "barcodedialog.h"
#include "ui_barcodedialog.h"
#include <QKeyEvent>
#include <QSqlQuery>

BarcodeDialog::BarcodeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BarcodeDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    ui->txt_code->installEventFilter(this);
}

BarcodeDialog::~BarcodeDialog()
{
    delete ui;
}

QString BarcodeDialog::getInput()
{
    return str;
}

bool BarcodeDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->txt_code) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *e = static_cast<QKeyEvent*>(event);
            if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
                str = ui->txt_code->text();
                done(1);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
