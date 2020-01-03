#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSqlDatabase mDatabase;
    void updateDB();

private slots:
    void on_PBnewOrder_clicked();

    void on_PBexit_clicked();


    void on_PBRefresh_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *tableModel;



};
#endif // MAINWINDOW_H
