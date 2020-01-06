#include "editorder.h"
#include "ui_editorder.h"
#include <QtDebug>
#include <QValidator>
#include <QSqlQuery>
#include <QMessageBox>

editorder::editorder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editorder)
{
    ui->setupUi(this);

    mainWin = dynamic_cast<MainWindow*>(parent);

    rowInfo = mainWin->getOrderInfo();

    //Fill all LineEdits with the information necessary
    ui->LEname->setText(rowInfo.value(1));
    ui->LEfac->setText(rowInfo.value(2));
    ui->LEic->setText(rowInfo.value(3));
    ui->LEfc->setText(rowInfo.value(4));
    ui->LEquant->setText(rowInfo.value(5));

    //Filling the type ComboBox with the currently available card types
    cardCode.append("HT2200");
    cardCode.append("HT2211");
    cardCode.append("HT2400");
    cardCode.append("HT2202");
    cardCode.append("HT2100");
    cardCode.append("HT2408");

    for (int i = 0; i < cardCode.size(); i++) {

        ui->CBtype->addItem(cardCode.value(i));
    }


    //Setting all the Line Edits of interest to be digits only
    QIntValidator *numsOnly = new QIntValidator(0, INT_MAX, this);

    ui->LEic->setValidator(numsOnly);
    ui->LEfc->setValidator(numsOnly);
    ui->LEfac->setValidator(numsOnly);
    ui->LEquant->setValidator(numsOnly);


}

editorder::~editorder()
{
    delete ui;
}

void editorder::on_PBcancel_clicked()
{
    close();
}



void editorder::on_LEic_textEdited(const QString &arg1)
{
    if(ui->LEquant->text() != "") {

        int initialCode = arg1.toInt();
        int quantity = ui->LEquant->text().toInt();

        ui->LEfc->setText(QString::number(initialCode + quantity));


    } else if (ui->LEfc->text() != "") {

        int initialCode = arg1.toInt();
        int finalCode = ui->LEfc->text().toInt();

        ui->LEquant->setText(QString::number(finalCode - initialCode));


    }
}

void editorder::on_LEfc_textEdited(const QString &arg1)
{
    if(ui->LEic->text() != "" && arg1.toInt() >= ui->LEic->text().toInt()) {

        int quantityLast = arg1.toInt();
        int quantityInitial = ui->LEic->text().toInt();

        ui->LEquant->setText(QString::number(quantityLast - quantityInitial));

    } else if (ui->LEic->text() != "" && arg1.toInt() < ui->LEic->text().toInt()) {

        ui->LEquant->setText("");
    }
}

void editorder::on_LEquant_textEdited(const QString &arg1)
{
    if(ui->LEic->text() != "") {

        int quantity = arg1.toInt();
        int initialCode = ui->LEic->text().toInt();

        ui->LEfc->setText(QString::number(initialCode + quantity));

    }
}

void editorder::on_PBedit_clicked()
{

    //making sure any of the required fields is not empty
    if (ui->LEname->text() == "" || ui->LEfac->text() == "" || ui->LEic->text() == "" || ui->LEfc->text() == "" || ui->LEquant->text() == "") {

        QMessageBox::critical(this, "Alert", "One or more of the required fields is empty, make sure everything is filled!");

        return;

    }

    bool wasFacChecked = false;

    bool wasCodeChecked = false;

    QSqlQuery query;

    if (query.exec("SELECT * FROM Cards ORDER BY ID DESC")) {

        while (query.next()) {

            if (!wasFacChecked || !wasCodeChecked) {

                //Facility code check
                if (QString::compare(query.value(2).toString(), ui->LEfac->text()) == 0 && QString::compare(query.value(1).toString(), ui->LEname->text()) != 0) {

                    QString tbox = query.value(1).toString() + " already has facility code " + query.value(2).toString() + ". This could cause an overlap in orders. Do you wish to continue?";

                    QMessageBox::StandardButton alert;
                    alert = QMessageBox::question(this, "Duplicate Code", tbox,
                                          QMessageBox::Yes|QMessageBox::No);


                    if (alert == QMessageBox::Yes && !wasFacChecked) {


                        wasFacChecked = true;


                    } else {


                        return;
                    }

                }


                //Initial-Final code check
                for (int i = query.value(3).toInt(); i < query.value(4).toInt(); i++) {

                    if (QString::compare(QString::number(i), ui->LEic->text()) == 0 || QString::compare(QString::number(i), ui->LEfc->text()) == 0) {


                        QString tbox = query.value(1).toString() + " is already using " + QString::number(i) + " as one of their codes. This could cause an overlap in orders. Do you wish to continue?";

                        QMessageBox::StandardButton alert;
                        alert = QMessageBox::question(this, "Duplicate Code", tbox,
                                              QMessageBox::Yes|QMessageBox::No);


                        if (alert == QMessageBox::Yes && !wasCodeChecked) {


                            wasCodeChecked = true;


                        } else {


                            return;

                        }


                    }

                }

            }


        }

    }


    //Passed all checks so edit the order
    performChange();



}

void editorder::on_CBtype_currentIndexChanged(const QString &arg1)
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


void editorder::performChange() {




}
