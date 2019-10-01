#ifndef EDITLEFTTABLEDIALOG_H
#define EDITLEFTTABLEDIALOG_H

#include <QDialog>
#include <QAction>
#include <QKeyEvent>
#include <QSqlError>
#include <QMessageBox>
#include <QTimer>

class QSqlTableModel;

namespace Ui {
class editLeftTableDialog;
}

class editLeftTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editLeftTableDialog(const QString &tableName, QWidget *parent = nullptr);
    ~editLeftTableDialog();

private slots:
    void on_closeBtn_clicked();
    void on_acceptBtn_clicked();
    void copy();
    void paste();
    void on_postBtn_clicked();
    void currentColumnChanged(const QModelIndex &mi);
    void autoSelectMagicColumn();

private:
    Ui::editLeftTableDialog *ui;
    QSqlTableModel *mModel;
    QAction *mActionAppendRow;
    QAction *mActionDeleteRow;
    QAction *mTurnIntoExcelData;
    QPoint parseCoordStringForTheTable(QString input);
    const int magicColumnIndexACoord = 10;
    const int magicColumnIndexBCoord = 11;
    const int magicColumnIndexCCoord = 12;
    const int magicColumnIndexDCoord = 13;
    bool submitData();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // EDITLEFTTABLEDIALOG_H
