#ifndef PIECHART_H
#define PIECHART_H

#include <QWidget>
#include <QString>

class PieChart : public QWidget
{
    Q_OBJECT
public:
    explicit PieChart(QWidget *parent = 0);
    void setData(QVector<QString> informations,QVector<double> values, QVector<QColor> colors);
protected:
    void paintEvent(QPaintEvent *);
private:
    QVector<QString> qvInformations;
    QVector<double> qvValues;
    QVector<QColor> qvColors;

signals:

public slots:

};

#endif // PIECHART_H
