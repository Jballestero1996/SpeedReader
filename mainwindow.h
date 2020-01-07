#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QList>
#include <QSortFilterProxyModel>

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
    QList<QString> getOrderInfo();

private slots:
    void on_PBnewOrder_clicked();

    void on_PBexit_clicked();


    void on_PBRefresh_clicked();

    void on_tableView_pressed(const QModelIndex &index);

    void on_PBeditOrder_clicked();


    void on_LEsearch_textEdited(const QString &arg1);

    void on_PBprintOrder_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *tableModel;
    QModelIndex curr;
    QList<QString> rowInfo;
    QString searchText;
    QSortFilterProxyModel *proxy;



};
#endif // MAINWINDOW_H
