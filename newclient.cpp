#include "newclient.h"
#include "ui_newclient.h"
#include <QMessageBox>
#include "QIntValidator"
#include "neworderwindow.h"

newclient::newclient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newclient)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    newOrder = qobject_cast<neworderwindow*>(parent);

    nameList = newOrder->getRidOfDuplicateList().toList();

    facilityMap = newOrder->getFacilityMap();


    //making sure only digits are allowed on fac code
    QIntValidator *numsOnly = new QIntValidator(0, INT_MAX, this);

    ui->LEfac->setValidator(numsOnly);






}


newclient::~newclient()
{
    delete ui;
}

void newclient::on_PBcancel_clicked()
{
    close();
}

void newclient::on_PBcreate_clicked()
{
    //Check if either box is empty

    if (ui->LEname->text() == "" || ui->LEfac->text() == "") {

        QMessageBox::critical(this, "Alert", "One or more of the required fields is empty, make sure everything is filled!");

        return;

    }


    bool isARepeat = false;

    if (nameList.contains(ui->LEname->text())) {


        QMessageBox::StandardButton alert;
        alert = QMessageBox::question(this, "Alert", "The customer " + ui->LEname->text() + " already exists in the client list. Do you wish to continue?");

        if (alert == QMessageBox::No) {

            return;

        }

        isARepeat = true;



    }

    bool facilityAccepted = false;

    for (int i = 0; i < nameList.size() && !facilityAccepted; i++) {

        if (QString::compare(facilityMap.value(nameList.value(i)), ui->LEfac->text()) == 0) {

            QString tbox =  nameList.value(i) + " already has the facility code " + facilityMap.value(nameList.value(i)) + ". This may cause an overlap between orders, do you wish to continue?";

            QMessageBox::StandardButton alert;
            alert = QMessageBox::question(this, "Duplicate Code", tbox,
                                  QMessageBox::Yes|QMessageBox::No);


            if (alert == QMessageBox::Yes) {


                facilityAccepted = true;



            } else {

                return;


            }


        }

    }

    facilityMap.insert(ui->LEname->text(), ui->LEfac->text());

    newOrder->updateFacilityCode(facilityMap, ui->LEname->text());

    newOrder->updateCustomerList(ui->LEname->text(), isARepeat);

    close();




}
