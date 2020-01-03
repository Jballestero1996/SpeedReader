#ifndef NEWORDERWINDOW_H
#define NEWORDERWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include "QSqlQuery"
#include "QSet"
#include <QList>
#include "mainwindow.h"

namespace Ui {
class neworderwindow;
}

class neworderwindow : public QDialog
{
    Q_OBJECT

public:
    explicit neworderwindow(QWidget *parent = nullptr);
    ~neworderwindow();

    QSet<QString> getRidOfDuplicateList();

    QMap<QString, QString> getFacilityMap();

    void updateCustomerList(QString, bool);

    void updateFacilityCode(QMap<QString, QString>, QString);



private slots:
    void on_PBcancel_clicked();

    void on_PBnewClient_clicked();

    void on_CBname_editTextChanged(const QString &arg1);

    void on_LEfc_textEdited(const QString &arg1);

    void on_LEquantity_textEdited(const QString &arg1);

    void on_LEic_textEdited(const QString &arg1);

    void on_PBcreate_clicked();

    void on_CBcardtype_currentIndexChanged(const QString &arg1);



private:
    Ui::neworderwindow *ui;
    QSqlDatabase mDatabase;
    QMap<QString, QString> facilityMap;
    QMap<QString, QString> finalMap;
    QMap<QString, QString> quantityMap;
    QList<QString> cardCode;
    MainWindow *firstWindow;
    QSet<QString> ridOfDuplicates;

    bool nameDuplicateDetector();

    void createOrder(QString name, QString fac, QString ic, QString fc, QString quantity, QString date, QString type);



};

#endif // NEWORDERWINDOW_H
