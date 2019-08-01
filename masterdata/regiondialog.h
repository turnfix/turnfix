#ifndef REGIONDIALOG_H
#define REGIONDIALOG_H

#include <QDialog>

class Region;
class EntityManager;
class StateModel;

namespace Ui {
class RegionDialog;
}

class RegionDialog : public QDialog
{
    Q_OBJECT
public:
    RegionDialog(Region *region, EntityManager *em, QWidget *parent = nullptr);
    ~RegionDialog() override;

private slots:
    void save();
    void addState();

private:
    Ui::RegionDialog *ui;
    EntityManager *m_em;
    StateModel *m_stateModel;
    Region *m_region;
};

#endif
