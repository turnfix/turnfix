#ifndef DLG_UPLOAD_H
#define DLG_UPLOAD_H

#include <QDialog>

namespace Ui {
    class Upload_Dialog;
}

class QNetworkAccessManager;
class QNetworkReply;

class Upload_Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Upload_Dialog(QWidget *parent = 0);
    ~Upload_Dialog();

private slots:
    void upload();
    void updateDialogLabel(QString text);
    void setStatus(int curr, int total);
    void uploadFiles(QStringList,QString);

    void uploadDone(QNetworkReply*);

private:
    Ui::Upload_Dialog *ui;
    QNetworkAccessManager *nw;
    QStringList fileList;
    bool errorShown;

};

#endif // DLG_UPLOAD_H
