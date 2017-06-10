#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "database.h"
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, protected Database
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_cancel_clicked();
    void on_connect_clicked();
    void on_log_out2_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_addBook_clicked();
    void on_BookView_clicked(const QModelIndex &index);
    void on_mod_book_clicked();
    void on_deleteBook_clicked();
    void on_recherche_clicked();
    void on_addUser_clicked();
    void on_DelUser_clicked();
    void on_modUser_clicked();
    void on_addAuthor_clicked();
    void on_DelAuthor_clicked();
    void on_modAuthor_clicked();
    void on_emp_clicked();
    void on_TakeBook_clicked(const QModelIndex &index);
    void on_rendreBook_clicked();

private:
    Ui::MainWindow *ui;
    QString user;
    QString titre,soustitre,domaine;
    QPixmap cover;
    int id;
    int id_book;
};

#endif // MAINWINDOW_H
