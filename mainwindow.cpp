#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "neworderwindow.h"
#include "editorder.h"
#include "QSqlDatabase"
#include "QSqlTableModel"
#include "QSqlError"
#include "QMessageBox"
#include <QAbstractItemModel>
#include <QPrinter>
#include <QPrintDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    mDatabase = QSqlDatabase::addDatabase("QODBC");
    mDatabase.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=C:\\Projects\\windshield cards.mdb");

    if(!mDatabase.open()) {

        QMessageBox::critical(this, "Error", mDatabase.lastError().text());
    }

    tableModel = new QSqlTableModel(this);
    tableModel->setTable("Cards");
    tableModel->select();



    //Used for the search function
    proxy = new QSortFilterProxyModel;

    proxy->setSourceModel(tableModel);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterKeyColumn(-1);
    proxy->setFilterFixedString("");

    ui->tableView->setModel(proxy);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_PBnewOrder_clicked()
{
    neworderwindow *newOrderWindow = new neworderwindow(this);

    newOrderWindow->setModal(true);

    newOrderWindow->exec();

}

void MainWindow::on_PBexit_clicked()
{

    mDatabase.close();
    close();
}



void MainWindow::on_PBRefresh_clicked()
{
    tableModel->select();

    tableModel->selectRow(0);

    ui->PBeditOrder->setEnabled(false);

    ui->PBprintOrder->setEnabled(false);

}

void MainWindow::updateDB() {

    tableModel->select();

    ui->tableView->sortByColumn(0,Qt::DescendingOrder);

    ui->tableView->selectRow(0);


    //Does a cleaning to add the newdly created for the list required for edits
    rowInfo.clear();

    for (int i = 0; i < 8; i++) {


        rowInfo.append(proxy->index(0,i).data().toString());


    }


    ui->PBeditOrder->setEnabled(true);
    ui->PBprintOrder->setEnabled(true);


}



void MainWindow::on_tableView_pressed(const QModelIndex &index)
{
    ui->PBeditOrder->setEnabled(true);

    ui->PBprintOrder->setEnabled(true);

    rowInfo.clear();

    curr = index;

    int test = index.row();


    for (int i = 0; i < tableModel->columnCount(); i++) {

        rowInfo.append(proxy->index(test, i).data().toString());


    }



}

void MainWindow::on_PBeditOrder_clicked()
{

    editorder *editOrderWindow = new editorder(this);

    editOrderWindow->setModal(true);

    editOrderWindow->exec();

}

QList<QString> MainWindow::getOrderInfo() {

    return rowInfo;

}


void MainWindow::on_LEsearch_textEdited(const QString &arg1)
{
    searchText = arg1;

    proxy->setFilterFixedString(searchText);


}

void MainWindow::on_PBprintOrder_clicked()
{

    QPrinter printer;

    printer.setPrinterName("desired printer name");

    QPrintDialog dialog (&printer, this);


    if (dialog.exec() == QDialog::Rejected) {

        return;

    }

    //need to create a file to set up the images of rapid pass and fac code + code


}
