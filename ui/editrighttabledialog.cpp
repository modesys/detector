#include "editrighttabledialog.h"
#include "ui_editrighttabledialog.h"
#include "../detector/src/edittablecomboboxdelegate.h"
#include <QtSql/QSqlTableModel>
#include <QAction>

editRightTableDialog::editRightTableDialog(const QString &tableName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editRightTableDialog)
{
    ui->setupUi(this);
    mActionLeftAppendRow = new QAction(QIcon(":"), tr("Add Row"), this);
    mActionLeftRemoveRow = new QAction(QIcon(":"), tr("Remove Row"), this);
    setWindowTitle(tableName);
    mRightModel = new QSqlTableModel(this);
    mRightModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mRightModel->setTable(tableName);
    mRightModel->select();
    ui->tableViewRight->setModel(mRightModel);
    ui->tableViewRight->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableViewRight->addActions({ mActionLeftAppendRow, mActionLeftRemoveRow });

    connect(mActionLeftAppendRow, &QAction::triggered, [&]() {
        mRightModel->insertRow(mRightModel->rowCount());
        mRightModel->submitAll();
    });
    connect(mActionLeftRemoveRow, &QAction::triggered, [&]() {
       mRightModel->removeRow(ui->tableViewRight->currentIndex().row());
    });
}

editRightTableDialog::~editRightTableDialog()
{
    delete ui;
}

void editRightTableDialog::on_acceptLeftBtn_clicked()
{
    mRightModel->submitAll();
}

void editRightTableDialog::on_closeLeftBtn_clicked()
{
    close();
}
