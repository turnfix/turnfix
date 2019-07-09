#include "checkdatabasedialog.h"
#include "dbchecker.h"
#include "ui_checkdatabasedialog.h"

CheckDatabaseDialog::CheckDatabaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckDatabaseDialog)
{
    ui->setupUi(this);
    ui->headerWidget->setHeaderText(this->windowTitle());

    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    check();
}

CheckDatabaseDialog::~CheckDatabaseDialog()
{
    delete ui;
}

void CheckDatabaseDialog::check()
{
    DBChecker *checker;

    checker = new DBChecker();

    ui->tableProgressBar->setValue(0);
    ui->columnProgressBar->setValue(0);

    connect(checker,SIGNAL(tableCount(int)), ui->tableProgressBar, SLOT(setMaximum(int)));
    connect(checker,SIGNAL(currentTable(QString)), ui->currentTableLabel, SLOT(setText(QString)));
    connect(checker, SIGNAL(tablesChecked(int)), ui->tableProgressBar, SLOT(setValue(int)));
    connect(checker, SIGNAL(columnFKCount(int)), ui->columnProgressBar, SLOT(setMaximum(int)));
    connect(checker, SIGNAL(columnsChecked(int)), ui->columnProgressBar, SLOT(setValue(int)));
    connect(checker, SIGNAL(finished()), this, SLOT(enableCloseButton()));
    checker->start();
}

void CheckDatabaseDialog::enableCloseButton()
{
    ui->buttonBox->setEnabled(true);
}
