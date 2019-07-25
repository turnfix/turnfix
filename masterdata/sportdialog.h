#ifndef SPORTDIALOG_H
#define SPORTDIALOG_H
#include <QDialog>

namespace Ui {
class SportDialog;
}

class Sport;
class EntityManager;

class SportDialog : public QDialog
{
    Q_OBJECT

public:
    SportDialog(Sport *sport, EntityManager *em, QWidget *parent = nullptr);
    ~SportDialog();

    Sport *sport();

private slots:
    void save();

private:
    Ui::SportDialog *ui;
    EntityManager *m_em;
    Sport *m_sport;
};
#endif
