#ifndef NEWCLIENT_H
#define NEWCLIENT_H

#include <QDialog>
#include "neworderwindow.h"
#include <QList>
#include <QMap>
#include <QSet>



namespace Ui {
class newclient;
}

class newclient : public QDialog
{
    Q_OBJECT

public:
    explicit newclient(QWidget *parent = nullptr);

//This constructor stays here as proof of a developing civilization
//newclient(QWidget *parent, QSet<QString> duplicates, QMap<QString, QString> facMap);
    ~newclient();

private slots:
    void on_PBcancel_clicked();

    void on_PBcreate_clicked();

    void on_LEfac_textEdited(const QString &arg1);

private:
    Ui::newclient *ui;

    neworderwindow *newOrder;

    QList<QString> nameList;

    QMap<QString, QString> facilityMap;


};

#endif // NEWCLIENT_H
