#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
    class SettingsWidget;
}

class QAbstractButton;

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();

private:
    Ui::SettingsWidget *ui;
    void loadData();
    void saveData();

private slots:
    void buttonController(QAbstractButton* button);
    void openDatabaseFile();
    void createDatabaseFile();
    void setupPostgresDatabase();
};

#endif // SETTINGSWIDGET_H
