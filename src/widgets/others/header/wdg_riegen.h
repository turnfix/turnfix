#ifndef WDG_RIEGEN_H
#define WDG_RIEGEN_H

#include <QWidget>

namespace Ui {
    class Riegen_Widget;
}

class Event;
class AssignmentTableModel;
class QTableView;

class Riegen_Widget : public QWidget {
    Q_OBJECT

public:
    explicit Riegen_Widget(Event *event, QString riege, QWidget *parent = nullptr);
    ~Riegen_Widget();
    AssignmentTableModel *re_model;
    void refreshDisziplinen();

private:
    Ui::Riegen_Widget *ui;
    Event *event;

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
