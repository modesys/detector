#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include "../detector/src/qcustomplot.h"

#include <QDialog>
#include <QVector>

namespace Ui {
class graphDialog;
}

class graphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit graphDialog(QWidget *parent = 0);
    ~graphDialog();

    void setData(std::vector<double> *newdata);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::graphDialog *ui;
    std::vector<double> *dataRange, *dataValues;
};

#endif // GRAPHDIALOG_H
