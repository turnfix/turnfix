#ifndef DLG_WK_H
#define DLG_WK_H
#include <QDialog>

namespace Ui {
class CompetitionDialog;
}

class Event;
class QStandardItemModel;

class CompetitionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompetitionDialog(Event *m_event, int edit = 0, QWidget *parent = nullptr);
    ~CompetitionDialog() override;

private slots:
    void initData();
    void save();
    void fillTable();
    void fillTable2();
    void enableOptions(int typ);
    void moveUp();
    void moveDown();
    void markGroup();

    void orderMoveUp();
    void orderMoveDown();
    void orderMoveLeft();
    void orderMoveRight();

private:
    Event *m_event;
    Ui::CompetitionDialog *ui;
    int editid;
    QStandardItemModel *model;
    QStandardItemModel *model2;
};
#endif
