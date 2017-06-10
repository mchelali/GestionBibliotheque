#include "adduser.h"
#include "ui_adduser.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
    ui->date->setDate(QDate::currentDate());
    ui->rang->addItems(QStringList()<<"Admin"<<"Etudiant"<<"Enseignent");
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_cancel_clicked()
{
    close();
}

void AddUser::on_save_clicked()
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("INSERT INTO Adherent ('Pseudo_ad','Password_ad','Rang_ad','Nom_ad','Prenom_ad','Adresse_ad','Tel_ad','Mail_ad','Date_Ins')"
                "VALUES (:pseudo, :password, :rang, :nom, :prenom, :adresse, :tel, :mail, :date)");
    qry.bindValue(":pseudo", ui->pseudo->text());
    qry.bindValue(":password", ui->password->text());
    qry.bindValue(":rang", ui->rang->currentText());
    qry.bindValue(":nom", ui->nom->text());
    qry.bindValue(":prenom", ui->prenom->text());
    qry.bindValue(":adresse", ui->adresse->text());
    qry.bindValue(":tel", ui->tel->text());
    qry.bindValue(":mail", ui->mail->text());
    qry.bindValue(":date", ui->date->date().toString("dddd dd MM yyyy"));

    if(!qry.exec()){
        qDebug() << qry.lastError() ;
    }else{
        qDebug() << "succes saved new user";
    }
    dbclose();
    close();
}
