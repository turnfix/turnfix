#ifndef POSTGRESSETUPWIZARD_H
#define POSTGRESSETUPWIZARD_H

#include <QWizard>

namespace Ui {
    class PostgresSetupWizard;
}

class PostgresSetupWizard : public QWizard
{
    Q_OBJECT

public:
    explicit PostgresSetupWizard(QWidget *parent = 0);
    ~PostgresSetupWizard();

private:
    Ui::PostgresSetupWizard *ui;

private slots:
    void prePageAction(int pageId);
    void updateStatus(int stage,bool successful,QString message);
};

#endif // POSTGRESSETUPWIZARD_H
