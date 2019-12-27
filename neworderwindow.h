#ifndef NEWORDERWINDOW_H
#define NEWORDERWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include "QSqlTableModel"

namespace Ui {
class neworderwindow;
}

class neworderwindow : public QDialog
{
    Q_OBJECT

public:
    explicit neworderwindow(QWidget *parent = nullptr);
    ~neworderwindow();

private slots:
    void on_PBcancel_clicked();

    void on_PBnewClient_clicked();

    void on_CBname_editTextChanged(const QString &arg1);

    void on_LEfc_textEdited(const QString &arg1);

    void on_LEquantity_textEdited(const QString &arg1);

    void on_LEic_textEdited(const QString &arg1);

private:
    Ui::neworderwindow *ui;
    QSqlTableModel *tableModel;
    QSqlDatabase mDatabase;
    QMap<QString, QString> typeMap;
    QMap<QString, QString> facilityMap;
    QMap<QString, QString> finalMap;


};

#endif // NEWORDERWINDOW_H
