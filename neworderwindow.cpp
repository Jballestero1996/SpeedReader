#include "neworderwindow.h"
#include "ui_neworderwindow.h"
#include "newclient.h"
#include "QSqlDatabase"
#include "QSqlError"
#include "QMessageBox"
#include "QSet"
#include "QMap"
#include "QIntValidator"
#include "QString"
#include <QDebug>
#include <QDate>

neworderwindow::neworderwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::neworderwindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    //Querying the names from the cards to fit into the drop down list
    QSet<QString> ridOfDuplicates;
    QSqlQuery query;

    if (query.exec("SELECT * FROM Cards ORDER BY RecordDate DESC")) {

        while(query.next()) {

            while(!ridOfDuplicates.contains(query.value(1).toString())) {

                 ui->CBname->addItem(query.value(1).toString());
                 ridOfDuplicates.insert(query.value(1).toString());

                 //Very rough approach but easy to work with since it stores all the values of interest for the last use
                 typeMap.insert(query.value(1).toString(),query.value(7).toString());
                 facilityMap.insert(query.value(1).toString(),query.value(2).toString());
                 finalMap.insert(query.value(1).toString(),query.value(4).toString());
                 quantityMap.insert(query.value(1).toString(), query.value(5).toString());

            }

        }


    } else {

        QMessageBox::critical(this, "Error", mDatabase.lastError().text());
    }


    //Necessary to make the first calculation
    on_CBname_editTextChanged(ui->CBname->currentText());


    //Setting all the Line Edits of interest to be digits only
    QIntValidator *numsOnly = new QIntValidator(0, INT_MAX, this);

    ui->LEic->setValidator(numsOnly);
    ui->LEfc->setValidator(numsOnly);
    ui->LEfac->setValidator(numsOnly);
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

    if(finalMap.contains(arg1) && quantityMap.contains(arg1)) {


        QString text = finalMap.value(arg1);

        ui->LEic->setText(QString::number(text.toInt() + 1));

        QString text2 = quantityMap.value(arg1);

        ui->LEquantity->setText(text2);

        on_LEquantity_textEdited(text2);

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

void neworderwindow::on_PBcreate_clicked()
{

    //making sure any of the required fields is not empty
    if (ui->CBname->currentText() == "" || ui->LEfac->text() == "" || ui->LEic->text() == "" || ui->LEfc->text() == "" || ui->LEquantity->text() == "") {

        QMessageBox::critical(this, "Alert", "One or more of the required fields is empty, make sure everything is filled!");

        return;

    }

    bool flag = false;

    bool orderCreated = false;

    bool orderCanceled = false;

    int biggestID = 0;

    QString date = QDate::currentDate().toString("d/M/yyyy");

    QSqlQuery plsWork(mDatabase);

    plsWork.clear();


    if(plsWork.exec("SELECT * FROM Cards ORDER BY ID DESC")) {

        while(plsWork.next()) {

            //Gets biggest ID on one go to create a new ID if needed
            if(!flag) {
            biggestID = plsWork.value(0).toInt() + 1;
            flag = true;
            }


            QString initialCodeTester = plsWork.value(3).toString();

            QString finalCodeTester = plsWork.value(4).toString();

            QString lineEditInitialCode = ui->LEic->text();

            QString lineEditFinalCode = ui->LEfc->text();


            for (int i = initialCodeTester.toInt(); i <= finalCodeTester.toInt(); i++) {

                if (orderCreated || orderCanceled) {

                    break;
                }

                int compareInitial= QString::compare(QString::number(i), lineEditInitialCode, Qt::CaseInsensitive);

                int compareFinal = QString::compare(QString::number(i), lineEditFinalCode, Qt::CaseInsensitive);

                if (compareInitial == 0 || compareFinal == 0) {

                    QString tbox = plsWork.value(1).toString() + " already is using the value " + QString::number(i) + " for one of their orders. This may cause an overlap between orders, do you wish to continue?";


                    QMessageBox::StandardButton alert;
                    alert = QMessageBox::question(this, "Duplicate Code", tbox,
                                          QMessageBox::Yes|QMessageBox::No);


                    if (alert == QMessageBox::Yes) {


                        createOrder(QString::number(biggestID), ui->CBname->currentText(), ui->LEfac->text(), ui->LEic->text(),
                                    ui->LEfc->text(), ui->LEquantity->text(), date, ui->LEtype->text());


                        orderCreated = true;

                        close();


                    } else {

                        orderCanceled = true;

                    }




                }


            }


        }


    }

    //if there is no repeat and no new order was created create the order
    if(!orderCreated && !orderCanceled) {

        createOrder(QString::number(biggestID), ui->CBname->currentText(), ui->LEfac->text(), ui->LEic->text(),
                    ui->LEfc->text(), ui->LEquantity->text(), date, ui->LEtype->text());


        close();


    }



}


void neworderwindow::createOrder(QString id, QString name, QString fac, QString ic, QString fc, QString quantity, QString date, QString type) {

//    QSqlQuery newAddition(mDatabase);

//    newAddition.clear();

//    newAddition.prepare("INSERT INTO Cards (""ID," "Costumer," "FCode," "InitCode," "FinalCode," "Quantity," "RecordDate," "Note)" "values(?, ?, ?, ?, ?, ?, ?, ?);");

//    newAddition.addBindValue(id);
//    newAddition.addBindValue(name);
//    newAddition.addBindValue(fac);
//    newAddition.addBindValue(ic);
//    newAddition.addBindValue(fc);
//    newAddition.addBindValue(quantity);
//    newAddition.addBindValue(date);
//    newAddition.addBindValue(type);

//    newAddition.exec();

//    tableModel->select();


}
