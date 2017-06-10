#include "modbook.h"
#include "ui_modbook.h"
#include <QFileDialog>
#include <QSqlError>
#include <QBuffer>

modbook::modbook(QWidget *parent, QString row) :
    QDialog(parent),
    ui(new Ui::modbook)
{
    ui->setupUi(this);

    QBuffer buffer(&cover);
    buffer.open(QIODevice::WriteOnly);
    ui->cover->pixmap()->save(&buffer, "PNG");

    qDebug() << row;

    dbConnect();
    QSqlQuery qry;
    qry.prepare("SELECT ID_auteur,Nom_auteur,Prenom_auteur FROM Auteur");
    if (qry.exec()){
        while(qry.next()){
            //qDebug() << qry.value(0).toString()+" "+qry.value(1).toString() << endl;
            ui->comboAuthor->addItem(qry.value(1).toString()+" "+qry.value(2).toString(), qry.value(0).toInt());
        }
    }

    qry.prepare("SELECT Titre,SousTitre,Domaine,nbr_exemplaire,Cover,ID_livre FROM Livres WHERE ID_livre=:info OR Titre=:info OR SousTitre=:info ");
    qry.bindValue(":info",row);
    qry.exec();

    qry.next();
    qDebug() << qry.value(0);
    idBook=qry.value(5).toInt();

    ui->title->setText(qry.value(0).toString());
    ui->subtitle->setText(qry.value(1).toString());
    ui->domaine->setText(qry.value(2).toString());
    ui->nbrExemp->setText(qry.value(3).toString());

    QByteArray array = qry.value(4).toByteArray();
    QPixmap cover= QPixmap();
    cover.loadFromData(array);

    ui->cover->setPixmap(cover);
    ui->cover->setScaledContents(true);

    dbclose();


}

modbook::~modbook()
{
    delete ui;
}

void modbook::on_cancel_clicked()
{
    close();
}

void modbook::on_seveMod_clicked()
{
    dbConnect();
    QSqlQuery qry;
    qry.prepare("UPDATE Livres "
                "SET Titre=:ti, SousTitre=:sti, Domaine=:do, nbr_exemplaire=:nbr, Note=:no, resumer=:res, Cover=:co "
                "WHERE ID_livre="+QString::number(idBook));
    qry.bindValue(":ti", ui->title->text());
    qry.bindValue(":sti", ui->subtitle->text());
    qry.bindValue(":do", ui->domaine->text());
    qry.bindValue(":nbr", ui->nbrExemp->text());
    qry.bindValue(":no", ui->noteSpin->text());
    qry.bindValue(":res", ui->desc->toPlainText());
    qry.bindValue(":co", cover);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }else{
        qDebug() << "Update succeful";
    }
    dbclose();
    close();
}

void modbook::on_addCover_clicked()
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
