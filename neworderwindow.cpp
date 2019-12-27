#include "neworderwindow.h"
#include "ui_neworderwindow.h"
#include "newclient.h"
#include "QSqlDatabase"
#include "QSqlError"
#include "QMessageBox"
#include "QSqlQuery"
#include "QSet"

neworderwindow::neworderwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::neworderwindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    //Querying the names from the cards to fit into the drop down list
    QSqlQuery query;
    QSet<QString> ridOfDuplicates;

    if (query.exec("SELECT * FROM Cards")) {

        while(query.next()) {

            while(!ridOfDuplicates.contains(query.value(1).toString())) {

                 ui->CBname->addItem(query.value(1).toString());
                 ridOfDuplicates.insert(query.value(1).toString());
            }

        }


    } else {

        QMessageBox::critical(this, "Error", mDatabase.lastError().text());
    }

}

neworderwindow::~neworderwindow()
{
    delete ui;
}

void neworderwindow::on_PBcancel_clicked()
{
    close();
}

void neworderwindow::on_PBnewClient_clicked()
{
     newclient *nc = new newclient();

     nc->setModal(true);

     nc->exec();
}
