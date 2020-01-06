#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "neworderwindow.h"
#include "editorder.h"
#include "QSqlDatabase"
#include "QSqlTableModel"
#include "QSqlError"
#include "QMessageBox"
#include <QtDebug>


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

    ui->tableView->setModel(tableModel);


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
}

void MainWindow::updateDB() {

    tableModel->select();

    ui->PBeditOrder->setEnabled(false);

}



void MainWindow::on_tableView_pressed(const QModelIndex &index)
{
    ui->PBeditOrder->setEnabled(true);

    rowInfo.clear();

    curr = index;

    int test = index.row();


    for (int i = 0; i < tableModel->columnCount(); i++) {

        rowInfo.append(tableModel->index(test, i).data().toString());


    }


    qDebug() << rowInfo << endl;


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
