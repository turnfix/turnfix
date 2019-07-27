#ifndef VENUEDIALOG_H
#define VENUEDIALOG_H
#include <QDialog>

namespace Ui {
class VenueDialog;
}

class Venue;
class EntityManager;

class VenueDialog : public QDialog
{
    Q_OBJECT

public:
    VenueDialog(Venue *venue, EntityManager *em, QWidget *parent = nullptr);
    ~VenueDialog();

    Venue *venue();

private slots:
    void save();

private:
    Ui::VenueDialog *ui;
    EntityManager *m_em;
    Venue *m_venue;
};
#endif
