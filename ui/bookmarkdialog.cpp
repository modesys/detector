#include "bookmarkdialog.h"
#include "ui_bookmarkdialog.h"

bookmarkDialog::bookmarkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookmarkDialog)
{
    ui->setupUi(this);
}

bookmarkDialog::~bookmarkDialog()
{
    delete ui;
}
