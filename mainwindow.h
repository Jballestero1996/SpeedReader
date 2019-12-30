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
    QSqlTableModel *tableModel;

private slots:
    void on_PBnewOrder_clicked();

    void on_PBexit_clicked();

private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
