#include "cuttingtooldialog.h"
#include "ui_cuttingtooldialog.h"

cuttingToolDialog::cuttingToolDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cuttingToolDialog)
{
    ui->setupUi(this);
}

cuttingToolDialog::~cuttingToolDialog()
{
    delete ui;
}
