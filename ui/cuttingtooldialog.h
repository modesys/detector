#ifndef CUTTINGTOOLDIALOG_H
#define CUTTINGTOOLDIALOG_H

#include <QDialog>

namespace Ui {
class cuttingToolDialog;
}

class cuttingToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cuttingToolDialog(QWidget *parent = nullptr);
    ~cuttingToolDialog();

private:
    Ui::cuttingToolDialog *ui;
};

#endif // CUTTINGTOOLDIALOG_H
