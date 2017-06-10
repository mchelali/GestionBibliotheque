#include "piechart.h"
#include <QPainter>
#include <QColor>
PieChart::PieChart(QWidget *parent) :
    QWidget(parent)
{
}

void PieChart::setData(QVector<QString> informations,QVector<double> values, QVector<QColor> colors)
{
    qvInformations = informations;
    qvValues = values;
    qvColors = colors;
    repaint();
    update();
}


void PieChart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    QRectF size;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    //size = QRectF(5, 5 ,this->width()-10, this->width()-10);

    double sum =0.0, startAng=0;
    double angle, endAng, percent;

    for(int i=0; i<qvValues.size(); i++){
        sum +=qvValues[i];
    }
    int x=this->width();
    for(int i=0; i<qvValues.size(); i++){
        x+=20;//interline entre les titre
        percent = qvValues[i] / sum;
        angle = percent *360.0;
        endAng = startAng+angle;
        painter.setBrush(qvColors[i]);
        painter.drawPie(0, 0, this->width(), this->width(), startAng*16, angle*16);
        painter.drawRect(2, x-10, 16, 12);
        painter.drawText(25, x, QString::number(percent*100)+" % : "+qvInformations[i]);
        startAng=endAng;
    }
}
