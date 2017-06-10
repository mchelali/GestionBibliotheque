#include "addbook.h"
#include "ui_addbook.h"
#include <QFileDialog>
#include <QString>
#include <QPixmap>
#include <QDebug>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QBuffer>

AddBook::AddBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBook)
{
    ui->setupUi(this);

    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT ID_auteur,Nom_auteur,Prenom_auteur FROM Auteur");
    if (qry.exec()){
        while(qry.next()){
            //qDebug() << qry.value(0).toString()+" "+qry.value(1).toString() << endl;
            ui->comboAuthor->addItem(qry.value(1).toString()+" "+qry.value(2).toString(), qry.value(0).toInt());
        }
    }
    dbclose();

    QBuffer buffer(&cover);
    buffer.open(QIODevice::WriteOnly);
    ui->cover->pixmap()->save(&buffer, "PNG");

}

AddBook::~AddBook()
{
    delete ui;
}

void AddBook::on_CancelBook_clicked()
{
    close();
}

void AddBook::on_addCover_clicked()
{
    QString coverPath = QFileDialog::getOpenFileName(this,tr("open file"),"/home", tr("Images (*.png *.jpeg *.jpg)"));

    QFile file(coverPath);
    if (!file.open(QIODevice::ReadOnly)) return;
    cover = file.readAll();

    QPixmap pixmap = QPixmap();
    pixmap.loadFromData(cover);
    ui->cover->setPixmap(pixmap);
    ui->cover->setScaledContents(true);

}

void AddBook::on_ADD_clicked()
{
    titre=ui->title->text();
    sousTitre=ui->subtitle->text();
    domaine=ui->domaine->text();
    nbr_exemp=ui->nbrExemp->text();
    defBook=ui->defBook->toPlainText();

    if (titre!= NULL && sousTitre!=NULL && domaine!=NULL && nbr_exemp!=NULL ){
        dbConnect();
        QSqlQuery qry;

        //INSERT INTO "main"."Livres" ("Titre","SousTitre","Domaine","nbr_exemplaire","Cover") VALUES (?1,?2,?3,?4,?5)

        qry.prepare("INSERT INTO Livres ('Titre', 'SousTitre', 'Domaine', 'nbr_exemplaire', 'Cover', 'resumer') "
                    "VALUES (:ti, :sousT, :dom, :exemp, :img, :def)");
        qry.bindValue(":ti", titre);
        qry.bindValue(":sousT", sousTitre);
        qry.bindValue(":dom", domaine);
        qry.bindValue(":exemp", nbr_exemp.toInt());
        qry.bindValue(":img", cover);
        qry.bindValue(":def", defBook);
        qry.exec();

        qry.prepare("SELECT COUNT(Titre) FROM Livres");
        qry.exec();
        int id_book;
        while(qry.next()) id_book=qry.value(0).toInt();

        int authorID=ui->comboAuthor->currentIndex()+1;
        qry.prepare("INSERT INTO Ecrit ('ID_auteur','ID_livre') "
                    "VALUES (:auteur,:book) ");
        qry.bindValue(":auteur",authorID);
        qry.bindValue(":book",id_book);
        qry.exec();
    }

    dbclose();
    close();

}



void AddBook::on_cancel_clicked()
{
    ui->AutorBox->setDisabled(true);
}

void AddBook::on_add_clicked()
{
    ui->AutorBox->setEnabled(true);
}

void AddBook::on_save_clicked()
{
    QString name_author=ui->name_autor->text();
    QString prenom=ui->prenom_author->text();
    QString paye=ui->paye_author->text();

    if(name_author!=NULL && prenom!=NULL && paye!=NULL){
        dbConnect();

        QSqlQuery qry;

        qDebug() << "tous les change sont remplis :)";
        //INSERT INTO "main"."Auteur" ("Nom_auteur","Prenom_auteur","Paye_auteur") VALUES (?1,?2,?3)
        qry.prepare("INSERT INTO 'Auteur' ('Nom_auteur', 'Prenom_auteur', 'Paye_auteur') "
                    "VALUES (:nom, :prenom, :paye)");
        qry.bindValue(":nom", name_author);
        qry.bindValue(":prenom", prenom);
        qry.bindValue(":paye", paye);
        qry.exec();

        ui->comboAuthor->addItem(name_author+" "+prenom);
        ui->AutorBox->setDisabled(true);
    }

    dbclose();

}
