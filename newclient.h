#ifndef NEWCLIENT_H
#define NEWCLIENT_H

#include <QDialog>

namespace Ui {
class newclient;
}

class newclient : public QDialog
{
    Q_OBJECT

public:
    explicit newclient(QWidget *parent = nullptr);
    ~newclient();

private slots:
    void on_PBcancel_clicked();

private:
    Ui::newclient *ui;
};

#endif // NEWCLIENT_H
