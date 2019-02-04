#ifndef WDG_RIEGEN_H
#define WDG_RIEGEN_H

#include <QWidget>

namespace Ui {
    class Riegen_Widget;
}

class QEinteilTableModel;
class QTableView;

class Riegen_Widget : public QWidget {
    Q_OBJECT

public:
    explicit Riegen_Widget(QString riege, QWidget *parent = 0);
    ~Riegen_Widget();
    QEinteilTableModel *re_model;
    void refreshDisziplinen();

private:
    Ui::Riegen_Widget *ui;

private slots:
    void request();
    void remove();
    void updateRiege();
    void saveFirst();
signals:
    void requestSelected(Riegen_Widget*);
    void removeSelected(Riegen_Widget*,QTableView*);
    void removeMe(Riegen_Widget*);
};

#endif // WDG_RIEGEN_H
