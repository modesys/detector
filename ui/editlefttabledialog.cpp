#include "editlefttabledialog.h"
#include "ui_editlefttabledialog.h"
#include "../detector/src/edittablecomboboxdelegate.h"
#include "../detector/src/leftcamparameters.h"
#include <QAction>
#include <QComboBox>
#include <QKeyEvent>
#include <QModelIndexList>
#include <QClipboard>
#include <QModelIndex>
#include <QDebug>
#include <QList>
#include <QSqlTableModel>
#include <QShortcut>
#include <QDebug>
#include <QModelIndex>
#include <QMessageBox>



editLeftTableDialog::editLeftTableDialog(const QString &tableName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editLeftTableDialog)
{
    ui->setupUi(this);
    mActionAppendRow = new QAction(QIcon(":"), tr("Add Row"), this);
    mActionDeleteRow = new QAction(QIcon(":"), tr("Delete Row"), this);
    mTurnIntoExcelData = new QAction(QIcon(":"), tr("Turn into Excel format"), this);

    mModel = new QSqlTableModel(this);
    mModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mModel->setTable(tableName);
    mModel->select();
    ui->tableViewLeft->setModel(mModel);

    connect(ui->tableViewLeft->selectionModel(), SIGNAL(currentColumnChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(currentColumnChanged(const QModelIndex &)));

    QShortcut *shortcut_copy = new QShortcut(QKeySequence::Copy, ui->tableViewLeft);
    connect(shortcut_copy, &QShortcut::activated, this, &editLeftTableDialog::copy);

    QShortcut *shortcut_paste = new QShortcut(QKeySequence::Paste, ui->tableViewLeft);
    connect(shortcut_paste, &QShortcut::activated, this, &editLeftTableDialog::paste);

    ui->tableViewLeft->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableViewLeft->addActions({mActionAppendRow, mActionDeleteRow, mTurnIntoExcelData});

    connect(mActionAppendRow, &QAction::triggered, [&]() {
        mModel->insertRow(mModel->rowCount());
        ui->tableViewLeft->setItemDelegateForColumn(6, new editTableComboBoxDelegate());
        ui->tableViewLeft->setItemDelegateForColumn(7, new editTableComboBoxDelegate());
        mModel->submitAll();
        ui->tableViewLeft->show();

    });
    connect(mActionDeleteRow, &QAction::triggered, [&]() {
       mModel->removeRow(ui->tableViewLeft->currentIndex().row());
    });

    connect(mTurnIntoExcelData, &QAction::triggered, [&]() {
        int row = -1, column = -1;
        QString reference;
        QString type;
        QModelIndex index;
        int rowModel = index.row();
        SelectionData currentData;

        for(int i = 0; i < ui->tableViewLeft->model()->columnCount(); i++)
        {
          if(ui->tableViewLeft->model()->headerData(i, Qt::Horizontal).toString() == "ACoord") {
              column = i;
              type = "ACoord";
              currentData.mACoord.setX(parseCoordStringForTheTable(index.sibling(row,10).data().toString()).x());
              currentData.mACoord.setY(parseCoordStringForTheTable(index.sibling(row,10).data().toString()).y());
              mModel->submitAll();
              ui->tableViewLeft->show();

          }
//          else if(ui->tableViewLeft->model()->headerData(i, Qt::Horizontal).toString() == "BCoord") {
//              column = i;
//              type = "BCoord";
//              currentData.mBCoord.setX(parseCoordStringForTheTable(index.sibling(row,11).data().toString()).x());
//              currentData.mBCoord.setY(parseCoordStringForTheTable(index.sibling(row,11).data().toString()).y());
//          }
//          else if(ui->tableViewLeft->model()->headerData(i, Qt::Horizontal).toString() == "CCoord") {
//              column = i;
//              type = "CCoord";
//              currentData.mCCoord.setX(parseCoordStringForTheTable(index.sibling(row,12).data().toString()).x());
//              currentData.mCCoord.setY(parseCoordStringForTheTable(index.sibling(row,12).data().toString()).y());
//          }
//          else if(ui->tableViewLeft->model()->headerData(i, Qt::Horizontal).toString() == "DCoord") {
//              column = i;
//              type = "DCoord";
//              currentData.mDCoord.setX(parseCoordStringForTheTable(index.sibling(row,13).data().toString()).x());
//              currentData.mDCoord.setY(parseCoordStringForTheTable(index.sibling(row,13).data().toString()).y());
//          }
        }

        QModelIndexList selection = ui->tableViewLeft->selectionModel()->selectedColumns();
        if (selection.count() > 0) {
            QModelIndex index = selection.at(0);
            row = index.row();
        }
        if(row < 0 || column < 0)
        {
            QMessageBox::warning(this, "Warning", "DoubleCheck - Didnt work");
        }
        else {
            reference = ui->tableViewLeft->model()->data(ui->tableViewLeft->model()->index(row,column)).toString();
            if(reference == "No Coordinates Present")
                return;
        }
    });

    qApp->installEventFilter(this);
}

editLeftTableDialog::~editLeftTableDialog()
{
    delete ui;
}

void editLeftTableDialog::on_acceptBtn_clicked()
{
    if(submitData())
        accept();
}

void editLeftTableDialog::on_postBtn_clicked()
{
    submitData();
}

void editLeftTableDialog::currentColumnChanged(const QModelIndex &mi)
{
    const int col = mi.column();
    if (col == magicColumnIndexACoord)
    {
       // Auto-select the entire column!  (Gotta schedule it be done later
       //  since it doesn't work if I just call selectColumn() directly at this point)
       QTimer::singleShot(100, this, SLOT(autoSelectMagicColumn()));
    }
    else if (col == magicColumnIndexBCoord)
    {
       // Auto-select the entire column!  (Gotta schedule it be done later
       //  since it doesn't work if I just call selectColumn() directly at this point)
       QTimer::singleShot(100, this, SLOT(autoSelectMagicColumn()));
    }
    else if (col == magicColumnIndexCCoord)
    {
       // Auto-select the entire column!  (Gotta schedule it be done later
       //  since it doesn't work if I just call selectColumn() directly at this point)
       QTimer::singleShot(100, this, SLOT(autoSelectMagicColumn()));
    }
    else if (col == magicColumnIndexDCoord)
    {
       // Auto-select the entire column!  (Gotta schedule it be done later
       //  since it doesn't work if I just call selectColumn() directly at this point)
       QTimer::singleShot(100, this, SLOT(autoSelectMagicColumn()));
    }
}

void editLeftTableDialog::autoSelectMagicColumn()
{
    // Double-check that the focus is still on the magic column index, in case the user moves fast
    if (ui->tableViewLeft->selectionModel()->currentIndex().column() == magicColumnIndexACoord)
    {
        ui->tableViewLeft->selectColumn(magicColumnIndexACoord);
    }
    else if (ui->tableViewLeft->selectionModel()->currentIndex().column() == magicColumnIndexBCoord)
    {
        ui->tableViewLeft->selectColumn(magicColumnIndexBCoord);
    }
    else if (ui->tableViewLeft->selectionModel()->currentIndex().column() == magicColumnIndexCCoord)
    {
        ui->tableViewLeft->selectColumn(magicColumnIndexCCoord);
    }
    else if (ui->tableViewLeft->selectionModel()->currentIndex().column() == magicColumnIndexDCoord)
    {
        ui->tableViewLeft->selectColumn(magicColumnIndexDCoord);
    }
}

QPoint editLeftTableDialog::parseCoordStringForTheTable(QString input)
{
    QPoint output;

    if(input.contains('('))
    {
        output.setX(input.remove('(').remove(')').remove(',').split(" ")[0].toInt());
        output.setY(input.remove('(').remove(')').remove(',').split(" ")[1].toInt());
    }
    else
    {
        output.setX(input.split(":")[0].toInt());
        output.setY(input.split(":")[1].toInt());
    }
    return output;
}

bool editLeftTableDialog::submitData()
{
    if(!mModel->submitAll())
    {
        QMessageBox::warning(this, "Submit Error", "Failed to write to table. Error was \"" + mModel->lastError().text() + "\"");
        return false;
    }
    else
    {
        return true;
    }

    return false;
}

void editLeftTableDialog::copy()
{
    QString text;
    QItemSelectionRange selection_range = ui->tableViewLeft->selectionModel()->selection().first();
    for(int i = selection_range.top(); i <= selection_range.bottom(); i++)
    {
        QStringList row_content;
        for(int j = selection_range.left(); j <= selection_range.right(); j++)
        {
            row_content << mModel->index(i,j).data().toString();
        }
        text += row_content.join(QChar('\t')) + QChar('\n');
    }
    QApplication::clipboard()->setText(text);
}

void editLeftTableDialog::paste()
{
    QModelIndex start = ui->tableViewLeft->selectionModel()->selectedIndexes().first();
    QString text = QApplication::clipboard()->text();
    int i = 0, j = 0;
    for(const QString & row_string: text.split("\n", QString::SkipEmptyParts))
    {
        j = 0;
        for(const QString & col: row_string.split("\t", QString::SkipEmptyParts))
        {
            QModelIndex ix = mModel->index(start.row() + i, start.column() + j);
            if(ix.isValid())
            {
                ui->tableViewLeft->model()->setData(ix, col);
                mModel->submit();
            }
            j++;
        }
        i++;
    }

    // Check if there are empty cells and if so throw and error
    for(int i=0; i < ui->tableViewLeft->model()->rowCount(); i++)
    {
        for(int j=0; j < ui->tableViewLeft->model()->columnCount(); j++)
        {
            bool flag = ui->tableViewLeft->model()->index(i,j).data().isNull();

            if (!flag) // the cell is not empty
            {
                continue;
            }
            else
            {
                QMessageBox::StandardButton q = QMessageBox::warning(this,"Attention","There is/are empty cell(s)");
                return;
                Q_UNUSED(q);
            }
        }
    }
}

void editLeftTableDialog::on_closeBtn_clicked()
{
    reject();
}

//void editLeftTableDialog::keyPressEvent(QKeyEvent *event)
//{
    // Copy from QTableView to an Excel or CSV file happens here and works well
//    QAbstractItemModel * model = ui->tableViewLeft->model();
//    QItemSelectionModel *selection = ui->tableViewLeft->selectionModel();
//    QModelIndexList indices = selection->selectedIndexes();
//    if(indices.isEmpty())
//        return;
//    QMap<int, bool> selectedColumnsMap;
//    Q_FOREACH (QModelIndex current, indices)
//    {
//        selectedColumnsMap[current.column()] = true;
//    }
//    QList<int> selectedColumns = selectedColumnsMap.uniqueKeys();
//    int minCol = selectedColumns.first();
//    // prepend headers for selected columns
//    QString selectedText;
//    Q_FOREACH (int column, selectedColumns)
//    {
//        selectedText += ui->tableViewLeft->model()->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
//        if(column != selectedColumns.last())
//            selectedText += QLatin1Char('\t');
//    }
//    selectedText += QLatin1Char('\n');
//    qSort(indices);
//    int lastRow = indices.first().row();
//    int lastColumn = minCol;
//    Q_FOREACH(QModelIndex current, indices)
//    {
//        if(current.row() != lastRow)
//        {
//            selectedText += QLatin1Char('\n');
//            lastColumn = minCol;
//            lastRow = current.row();
//        }
//        if(current.column() != lastColumn)
//        {
//            for(int i = 0; i < current.column() - lastColumn; ++i)
//                selectedText += QLatin1Char('\t');
//            lastColumn = current.column();
//        }
//        selectedText += ui->tableViewLeft->model()->data(current).toString();
//    }
//    selectedText += QLatin1Char('\n');
//    QApplication::clipboard()->setText(selectedText);
//}

bool editLeftTableDialog::eventFilter(QObject *obj, QEvent *event)
{
//    if(obj == ui->tableViewLeft && event->type() == QEvent::KeyPress) {
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        if(keyEvent->key() == Qt::Key_Shift) {
//            QModelIndex in = ui->tableViewLeft->currentIndex();
//            ui->tableViewLeft->model()->setData(in, "");
//        }
//    }
    if(obj == ui->tableViewLeft && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyCanc = static_cast<QKeyEvent *>(event);
        if(keyCanc->key() == Qt::Key_Delete) {
            QList<QModelIndex> index = ui->tableViewLeft->selectionModel()->selectedIndexes();
            for(int i = 0; i<index.size(); i++) {
                ui->tableViewLeft->model()->setData(index.at(i), "");
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

