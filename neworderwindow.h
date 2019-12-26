#ifndef NEWORDERWINDOW_H
#define NEWORDERWINDOW_H

#include <QDialog>

namespace Ui {
class neworderwindow;
}

class neworderwindow : public QDialog
{
    Q_OBJECT

public:
    explicit neworderwindow(QWidget *parent = nullptr);
    ~neworderwindow();

private slots:
    void on_PBcancel_clicked();

    void on_PBnewClient_clicked();

private:
    Ui::neworderwindow *ui;
};

#endif // NEWORDERWINDOW_H
