#include "moduser.h"
#include "ui_moduser.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ModUser::ModUser(QWidget *parent, QString val) :
    QDialog(parent),
    ui(new Ui::ModUser)
{
    ui->setupUi(this);
    ui->date->setDate(QDate::currentDate());
    ui->rang->addItems(QStringList()<<"Admin"<<"Etudiant"<<"Enseignent");

    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT Pseudo_ad,Password_ad,Rang_ad,Nom_ad,Prenom_ad,Adresse_ad,Tel_ad,Mail_ad,ID_adherent From Adherent "
                "WHERE (ID_adherent = :row OR Pseudo_ad = :row OR Password_ad = :row OR Nom_ad = :row OR Prenom_ad = :row OR Adresse_ad = :row OR Tel_ad = :row OR Mail_ad = :row)");
    qry.bindValue(":row", val);
    if((!qry.exec())){
        qDebug() << qry.lastError();
    }
    qry.next();
    ui->pseudo->setText(qry.value(0).toString());
    ui->password->setText(qry.value(1).toString());
    ui->nom->setText(qry.value(3).toString());
    ui->prenom->setText(qry.value(4).toString());
    ui->adresse->setText(qry.value(5).toString());
    ui->tel->setText(qry.value(6).toString());
    ui->mail->setText(qry.value(7).toString());
    idAdherent = qry.value(8).toInt();

    qDebug() << idAdherent;

    dbclose();
}

ModUser::~ModUser()
{
    delete ui;
}

void ModUser::on_pushButton_clicked()
{
    close();
}

void ModUser::on_pushButton_2_clicked()
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("UPDATE Adherent"
                "SET Pseudo_ad=:pseudo, Password_ad=:pass, Rang_ad=:rang, Nom_ad=:nom, Prenom_ad=:prenom, Adresse_ad=:adresse, Tel_ad=:tel, Mail_ad=:mail, Date_Ins=:date"
                "WHERE ID_adherent="+QString::number(idAdherent));
    qry.bindValue(":pseudo",ui->pseudo->text());
    qry.bindValue(":pass", ui->password->text());
    qry.bindValue(":rang",ui->rang->currentText());
    qry.bindValue(":nom", ui->nom->text());
    qry.bindValue(":prenom", ui->prenom->text());
    qry.bindValue(":adresse", ui->adresse->text());
    qry.bindValue(":tel", ui->tel->text());
    qry.bindValue(":mail", ui->mail->text());
    qry.bindValue(":date", ui->date->date().toString("dddd dd MM yyyy"));

    if(!qry.exec()) qDebug() << qry.lastError();
    dbclose();

    close();

}
