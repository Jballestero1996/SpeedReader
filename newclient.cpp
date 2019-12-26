#include "newclient.h"
#include "ui_newclient.h"

newclient::newclient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newclient)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

newclient::~newclient()
{
    delete ui;
}

void newclient::on_PBcancel_clicked()
{
    close();
}
