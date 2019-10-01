#include "graphdialog.h"
#include "ui_graphdialog.h"
#include <algorithm>

graphDialog::graphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graphDialog)
{
    ui->setupUi(this);

    dataRange = new std::vector<double>;
    dataValues = new std::vector<double>;

    ui->widget->addGraph();
    ui->widget->xAxis->setLabel("Position");
    ui->widget->yAxis->setLabel("Error");

}

graphDialog::~graphDialog()
{
    delete ui;
}

void graphDialog::setData(std::vector<double> *newdata)
{
    dataRange->clear();
    dataValues = newdata;
    for(int i=0;i<newdata->size();i++)
        dataRange->push_back(i);

    double max = *std::max_element(dataValues->begin(), dataValues->end());
    double min = *std::min_element(dataValues->begin(), dataValues->end());

    ui->widget->graph(0)->setData(QVector<double>::fromStdVector(*dataRange), QVector<double>::fromStdVector(*dataValues));
    ui->widget->xAxis->setRange(0, dataRange->size());
    ui->widget->yAxis->setRange(min, max);
    ui->widget->replot();
}

void graphDialog::on_buttonBox_accepted()
{
    accept();
}

void graphDialog::on_buttonBox_rejected()
{
    reject();
}
