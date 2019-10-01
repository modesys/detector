#include "edittablecomboboxdelegate.h"
#include <QComboBox>

editTableComboBoxDelegate::editTableComboBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

editTableComboBoxDelegate::~editTableComboBoxDelegate()
{
}

QWidget *editTableComboBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & option ,
    const QModelIndex & index ) const
{
    // ComboBox ony in column 6 and 7
        if (index.column() != 6 && index.column() != 7)
            return QStyledItemDelegate::createEditor(parent, option, index);
            // Create the combobox and populate it
            QComboBox* cb = new QComboBox(parent);
            int row = index.row();
            cb->addItem(QString("Rock").arg(row));
            cb->addItem(QString("Boulder").arg(row));
            cb->addItem(QString("Cobble").arg(row));
            cb->addItem(QString("Pebble").arg(row));
            cb->addItem(QString("Sand").arg(row));
            cb->addItem(QString("Mud").arg(row));
            return cb;

}

void editTableComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
//    QString value = index.model()->data(index, Qt::EditRole).toString();

    if (QComboBox* cb = qobject_cast<QComboBox*>(editor)) {
        // get the index of the text in the combobox that matches the current value of the itenm
        QString currentText = index.data(Qt::EditRole).toString();
        int cbIndex = cb->findText(currentText);
        // if it is valid, adjust the combobox
        if (cbIndex >= 0)
            cb->setCurrentIndex(cbIndex);
        } else {
            QStyledItemDelegate::setEditorData(editor, index);
        }
}

void editTableComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor))
        // save the current text of the combo box as the current value of the item
        model->setData(index, cb->currentText(), Qt::EditRole);
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

void editTableComboBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    editor->setGeometry(option.rect);
    (void) index;
}
