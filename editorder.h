#ifndef EDITORDER_H
#define EDITORDER_H

#include <QDialog>
#include <QList>
#include "mainwindow.h"

namespace Ui {
class editorder;
}

class editorder : public QDialog
{
    Q_OBJECT

public:
    explicit editorder(QWidget *parent = nullptr);
    ~editorder();

private slots:
    void on_PBcancel_clicked();

    void on_PBedit_clicked();

    void on_LEic_textEdited(const QString &arg1);

    void on_LEfc_textEdited(const QString &arg1);

    void on_LEquant_textEdited(const QString &arg1);

    void on_LEfac_textEdited(const QString &arg1);

    void on_CBtype_currentTextChanged(const QString &arg1);

private:
    Ui::editorder *ui;
    QList<QString> rowInfo;
    MainWindow * mainWin;
    QList<QString> cardCode;
    bool wasFacChecked;
    bool wasCodeChecked;

    void performChange();
};

#endif // EDITORDER_H
