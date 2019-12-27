#include "neworderwindow.h"
#include "ui_neworderwindow.h"
#include "newclient.h"
#include "QSqlDatabase"
#include "QSqlError"
#include "QMessageBox"
#include "QSqlQuery"
#include "QSet"
#include "QMap"
#include "QIntValidator"
#include "QString"

neworderwindow::neworderwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::neworderwindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    //Querying the names from the cards to fit into the drop down list
    QSqlQuery query;
    QSet<QString> ridOfDuplicates;


    if (query.exec("SELECT * FROM Cards ORDER BY RecordDate DESC")) {

        while(query.next()) {

            while(!ridOfDuplicates.contains(query.value(1).toString())) {

                 ui->CBname->addItem(query.value(1).toString());
                 ridOfDuplicates.insert(query.value(1).toString());

                 typeMap.insert(query.value(1).toString(),query.value(7).toString());
                 facilityMap.insert(query.value(1).toString(),query.value(2).toString());
                 finalMap.insert(query.value(1).toString(),query.value(4).toString());

            }

        }


    } else {

        QMessageBox::critical(this, "Error", mDatabase.lastError().text());
    }

    QIntValidator *numsOnly = new QIntValidator(0, INT_MAX, this);

    ui->LEic->setValidator(numsOnly);
    ui->LEfc->setValidator(numsOnly);
    ui->LEfac->setValidator(numsOnly);
    ui->LEserial->setValidator(numsOnly);
    ui->LEquantity->setValidator(numsOnly);

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

void neworderwindow::on_CBname_editTextChanged(const QString &arg1)
{

    if(facilityMap.contains(arg1)) {

        QString text = facilityMap.value(arg1);

        ui->LEfac->setText(text);

    }

    if(finalMap.contains(arg1)) {

        QString text = finalMap.value(arg1);

        ui->LEic->setText(text);
    }

    if(typeMap.contains(arg1)) {

        QString text = typeMap.value(arg1);

        ui->LEtype->setText(text);
    }


}

void neworderwindow::on_LEfc_textEdited(const QString &arg1)
{

    if(ui->LEic->text() != "" && arg1.toInt() >= ui->LEic->text().toInt()) {

        int quantityLast = arg1.toInt();
        int quantityInitial = ui->LEic->text().toInt();

        ui->LEquantity->setText(QString::number(quantityLast - quantityInitial));

    } else if (ui->LEic->text() != "" && arg1.toInt() < ui->LEic->text().toInt()) {

        ui->LEquantity->setText("");
    }

}

void neworderwindow::on_LEquantity_textEdited(const QString &arg1)
{

    if(ui->LEic->text() != "") {

        int quantity = arg1.toInt();
        int initialCode = ui->LEic->text().toInt();

        ui->LEfc->setText(QString::number(initialCode + quantity));

    }

}

void neworderwindow::on_LEic_textEdited(const QString &arg1)
{
    if(ui->LEquantity->text() != "") {

        int initialCode = arg1.toInt();
        int quantity = ui->LEquantity->text().toInt();

        ui->LEfc->setText(QString::number(initialCode + quantity));


    } else if (ui->LEfc->text() != "") {

        int initialCode = arg1.toInt();
        int finalCode = ui->LEfc->text().toInt();

        ui->LEquantity->setText(QString::number(finalCode - initialCode));


    }
}
