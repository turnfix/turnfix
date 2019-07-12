#include "checkdatabasedialog.h"
#include "model/dbchecker.h"
#include "model/entity/abstractconnection.h"
#include "ui_checkdatabasedialog.h"

CheckDatabaseDialog::CheckDatabaseDialog(AbstractConnection *connection, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CheckDatabaseDialog)
{
    ui->setupUi(this);
    ui->headerWidget->setHeaderText(this->windowTitle());

    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    DBChecker *checker = new DBChecker(connection);

    ui->tableProgressBar->setValue(0);
    ui->columnProgressBar->setValue(0);

    connect(checker, &DBChecker::tableCount, ui->tableProgressBar, &QProgressBar::setMaximum);
    connect(checker, &DBChecker::columnFKCount, ui->columnProgressBar, &QProgressBar::setMaximum);
    connect(checker, &DBChecker::tablesChecked, ui->tableProgressBar, &QProgressBar::setValue);
    connect(checker, &DBChecker::columnsChecked, ui->columnProgressBar, &QProgressBar::setValue);
    connect(checker, &DBChecker::currentTable, ui->currentTableLabel, &QLabel::setText);
    connect(checker, &DBChecker::finished, this, &CheckDatabaseDialog::enableCloseButton);

    checker->start();
}

CheckDatabaseDialog::~CheckDatabaseDialog()
{
    delete ui;
}

void CheckDatabaseDialog::enableCloseButton()
{
    ui->buttonBox->setEnabled(true);
}
