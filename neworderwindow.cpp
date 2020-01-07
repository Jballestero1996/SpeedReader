#include "neworderwindow.h"
#include "ui_neworderwindow.h"
#include "newclient.h"
#include "QSqlError"
#include "QMessageBox"
#include "QMap"
#include "QIntValidator"
#include "QString"
#include <QDate>
#include <QPixmap>



neworderwindow::neworderwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::neworderwindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    firstWindow = qobject_cast<MainWindow*>(parent);
    facWasChecked = false;

    //Querying the names from the cards to fit into the drop down list
    QSqlQuery query;

    if (query.exec("SELECT * FROM Cards ORDER BY RecordDate DESC")) {

        while(query.next()) {

            while(!ridOfDuplicates.contains(query.value(1).toString())) {

                 ui->CBname->addItem(query.value(1).toString());
                 ridOfDuplicates.insert(query.value(1).toString());

                 //Very rough approach but easy to work with since it stores all the values of interest from the last use
                 facilityMap.insert(query.value(1).toString(),query.value(2).toString());
                 finalMap.insert(query.value(1).toString(),query.value(4).toString());
                 quantityMap.insert(query.value(1).toString(), query.value(5).toString());



            }

        }


    } else {

        QMessageBox::critical(this, "Error", mDatabase.lastError().text());
    }


    //Hard coded the most popular card types to show images
    cardCode.append("HT2200");
    cardCode.append("HT2211");
    cardCode.append("HT2400");
    cardCode.append("HT2202");
    cardCode.append("HT2100");
    cardCode.append("HT2408");

    for (int i = 0; i < cardCode.size(); i++) {

        ui->CBcardtype->addItem(cardCode.value(i));
    }


    //Necessary to make the first calculation
    on_CBname_editTextChanged(ui->CBname->currentText());
    on_CBcardtype_currentIndexChanged(ui->CBcardtype->currentText());


    //Setting all the Line Edits of interest to be digits only
    QIntValidator *numsOnly = new QIntValidator(0, INT_MAX, this);
    QIntValidator *facNum = new QIntValidator(0, 255, this);

    ui->LEic->setValidator(numsOnly);
    ui->LEfc->setValidator(numsOnly);
    ui->LEfac->setValidator(facNum);
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
     newclient *nc = new newclient(this);

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

    bool orderCreated = false;

    bool orderCanceled = false;

    QString date = QDate::currentDate().toString("d/M/yyyy");

    QSqlQuery plsWork(mDatabase);

    plsWork.clear();

    if(plsWork.exec("SELECT * FROM Cards ORDER BY ID DESC")) {

        while(plsWork.next()) {

            QString initialCodeTester = plsWork.value(3).toString();

            QString finalCodeTester = plsWork.value(4).toString();

            QString lineEditInitialCode = ui->LEic->text();

            QString lineEditFinalCode = ui->LEfc->text();


            //Checks if more than 1 facility code match if they haven't been checked in the newclient class
            if (!facWasChecked) {

                if (facilityMap.contains(plsWork.value(1).toString()) && !ridOfDuplicates.contains(ui->CBname->currentText())) {

                    QString tbox = plsWork.value(1).toString() + " already has facility code " + facilityMap.value(plsWork.value(1).toString()) + ". This could cause an overlap in orders. Do you wish to continue?";

                    QMessageBox::StandardButton alert;
                    alert = QMessageBox::question(this, "Duplicate Code", tbox,
                                          QMessageBox::Yes|QMessageBox::No);


                    if (alert == QMessageBox::Yes) {


                        facWasChecked = true;


                    } else {


                        return;
                    }



                }

            }


            //Check for Initial or Final Code similarities
            for (int i = initialCodeTester.toInt(); i <= finalCodeTester.toInt(); i++) {

                if (orderCreated || orderCanceled) {

                    return;
                }

                int compareInitial= QString::compare(QString::number(i), lineEditInitialCode, Qt::CaseInsensitive);

                int compareFinal = QString::compare(QString::number(i), lineEditFinalCode, Qt::CaseInsensitive);

                if (compareInitial == 0 || compareFinal == 0) {

                    QString tbox = plsWork.value(1).toString() + " already is using the value " + QString::number(i) + " for one of their orders. This may cause an overlap between orders, do you wish to continue?";


                    QMessageBox::StandardButton alert;
                    alert = QMessageBox::question(this, "Duplicate Code", tbox,
                                          QMessageBox::Yes|QMessageBox::No);


                    if (alert == QMessageBox::Yes) {

                        if (nameDuplicateDetector()) {

                            createOrder(ui->CBname->currentText(), ui->LEfac->text(), ui->LEic->text(),
                                        ui->LEfc->text(), ui->LEquantity->text(), date, ui->CBcardtype->currentText());


                            orderCreated = true;



                        } else {

                            orderCanceled = true;
                        }



                    } else {

                        orderCanceled = true;

                    }


                }


            }


        }


    }


    if (orderCreated || orderCanceled || !nameDuplicateDetector()) {

        return;
    }


    //if there is no repeat and no new order was created create the order
    if(!orderCreated && !orderCanceled) {

        createOrder(ui->CBname->currentText(), ui->LEfac->text(), ui->LEic->text(),
                    ui->LEfc->text(), ui->LEquantity->text(), date, ui->CBcardtype->currentText());


    }


}


void neworderwindow::createOrder(QString name, QString fac, QString ic, QString fc, QString quantity, QString date, QString type) {

    QSqlQuery newAddition(mDatabase);

    newAddition.clear();

    newAddition.prepare("INSERT INTO Cards (Costumer, FCode, InitCode, FinalCode, Quantity, RecordDate, CardType) VALUES (?,?,?,?,?,?,?);");

    newAddition.addBindValue(name);
    newAddition.addBindValue(fac);
    newAddition.addBindValue(ic);
    newAddition.addBindValue(fc);
    newAddition.addBindValue(quantity);
    newAddition.addBindValue(date);
    newAddition.addBindValue(type);

    newAddition.exec();

    close();

    firstWindow->updateDB();



}


void neworderwindow::on_CBcardtype_currentIndexChanged(const QString &arg1)
{

    bool edited = false;

    for (int i = 0; i < cardCode.size(); i++){

        if(arg1.contains(cardCode.value(i))) {

            edited = true;

            //Change this to a switch case in the future
            if (QString::compare(cardCode.value(i), "HT2200") == 0) {

                QPixmap image("C:\\Projects\\HT2200.png");
                ui->LBimage->setPixmap(image.scaled(150, 150, Qt::KeepAspectRatio));

            } else if (QString::compare(cardCode.value(i), "HT2211") == 0) {

                QPixmap image("C:\\Projects\\HT2211.png");
                ui->LBimage->setPixmap(image.scaled(150, 150, Qt::KeepAspectRatio));

            } else if (QString::compare(cardCode.value(i), "HT2400") == 0) {

                QPixmap image("C:\\Projects\\HT2400.png");
                ui->LBimage->setPixmap(image.scaled(150, 150, Qt::KeepAspectRatio));

            } else if (QString::compare(cardCode.value(i), "HT2202") == 0) {

                QPixmap image("C:\\Projects\\HT2202.png");
                ui->LBimage->setPixmap(image.scaled(150, 150, Qt::KeepAspectRatio));

            } else if (QString::compare(cardCode.value(i), "HT2100") == 0) {

                QPixmap image("C:\\Projects\\HT2100.png");
                ui->LBimage->setPixmap(image.scaled(150, 150, Qt::KeepAspectRatio));

            }   else if (QString::compare(cardCode.value(i), "HT2408") == 0) {

                QPixmap image("C:\\Projects\\HT2408.png");
                ui->LBimage->setPixmap(image.scaled(150, 150, Qt::KeepAspectRatio));

            }



        }


    }

    if (!edited) {

            QPixmap image("C:\\Projects\\HT2100.png");
            ui->LBimage->setPixmap(image.scaled(0, 0, Qt::KeepAspectRatio));


    }


}

QSet<QString> neworderwindow::getRidOfDuplicateList() {

    return ridOfDuplicates;

}

void neworderwindow::updateCustomerList(QString updated, bool isARepeat) {

    if (!isARepeat) {


        ridOfDuplicates.insert(updated);

        ui->CBname->addItem(updated);

        ui->CBname->setCurrentText(updated);



    }


    on_CBname_editTextChanged(updated);


}

QMap<QString, QString> neworderwindow::getFacilityMap() {

    return facilityMap;

}

void neworderwindow::updateFacilityCode(QMap<QString, QString> newMap, QString key) {

    facilityMap.insert(key, newMap.value(key));


}

bool neworderwindow::nameDuplicateDetector() {

    if (!ridOfDuplicates.contains(ui->CBname->currentText()))  {

        QString tbox = ui->CBname->currentText() + " is a new Client that has not been registered. Do you wish to continue?";

        QMessageBox::StandardButton alert;
        alert = QMessageBox::question(this, "Duplicate Code", tbox,
                              QMessageBox::Yes|QMessageBox::No);


        if (alert == QMessageBox::Yes) {


            return true;



        } else {

            return false;


        }

    }


    return true;

}


void neworderwindow::facAccepted(bool isAccepted) {

   facWasChecked = isAccepted;


}

void neworderwindow::setCodesForNewClient() {


    ui->LEic->setText("1");
    ui->LEfc->setText("2");
    ui->LEquantity->setText("1");

}

void neworderwindow::on_LEfac_textEdited(const QString &arg1)
{
    if (arg1.toInt() > 255) {

        QString validFac = arg1.at(0);

        validFac += arg1.at(1);

        ui->LEfac->setText(validFac);


    }
}
