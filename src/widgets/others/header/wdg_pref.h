#ifndef WDG_PREF_H
#define WDG_PREF_H

#include <QWidget>

namespace Ui {
    class Preferences_Widget;
}

class Preferences_Widget : public QWidget {
    Q_OBJECT

public:
    explicit Preferences_Widget(QWidget *parent = 0);
    ~Preferences_Widget();

private:
    Ui::Preferences_Widget *ui;

private slots:
    void updatePrefs();
    void newDbTyp(int);

signals:
    void dbTypChanged(int);
};

#endif // WDG_PREF_H
