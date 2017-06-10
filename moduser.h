#ifndef MODUSER_H
#define MODUSER_H

#include <QDialog>
#include "database.h"
#include <QString>

namespace Ui {
class ModUser;
}

class ModUser : public QDialog, protected Database
{
    Q_OBJECT

public:
    explicit ModUser(QWidget *parent = 0, QString val="");
    ~ModUser();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ModUser *ui;
    int idAdherent;
};

#endif // MODUSER_H
