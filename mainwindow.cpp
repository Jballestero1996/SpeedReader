#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "neworderwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_PBnewOrder_clicked()
{
    neworderwindow *newOrderWindow = new neworderwindow();

    newOrderWindow->setModal(true);

    newOrderWindow->exec();

}

void MainWindow::on_PBexit_clicked()
{
    close();
}
