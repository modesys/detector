#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <QDialog>

namespace Ui {
class bookmarkDialog;
}

class bookmarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit bookmarkDialog(QWidget *parent = nullptr);
    ~bookmarkDialog();

private:
    Ui::bookmarkDialog *ui;
};

#endif // BOOKMARKDIALOG_H
