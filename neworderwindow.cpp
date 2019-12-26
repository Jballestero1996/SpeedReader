#include "neworderwindow.h"
#include "ui_neworderwindow.h"
#include "newclient.h"
neworderwindow::neworderwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::neworderwindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
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
