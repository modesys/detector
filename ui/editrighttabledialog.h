#ifndef EDITRIGHTTABLEDIALOG_H
#define EDITRIGHTTABLEDIALOG_H

#include <QDialog>
#include <QtSql/QSqlTableModel>
#include <QAction>

namespace Ui {
class editRightTableDialog;
}

class editRightTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editRightTableDialog(const QString &tableName, QWidget *parent = nullptr);
    ~editRightTableDialog();

private slots:
    void on_acceptLeftBtn_clicked();
    void on_closeLeftBtn_clicked();

private:
    Ui::editRightTableDialog *ui;
    QSqlTableModel *mRightModel;
    QAction *mActionLeftAppendRow;
    QAction *mActionLeftRemoveRow;

};

#endif // EDITRIGHTTABLEDIALOG_H
