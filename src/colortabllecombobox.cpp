#include "colortabllecombobox.h"
#include <QComboBox>

ColorTablleComboBox::ColorTablleComboBox(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

ColorTablleComboBox::~ColorTablleComboBox()
{
}

QWidget *ColorTablleComboBox::createEditor(QWidget *parent, const QStyleOptionViewItem & option ,
    const QModelIndex & index ) const
{
            (void) option;
            (void) index;
            return new QComboBox(parent);
}

void ColorTablleComboBox::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor))
    {
        // get the index of the text in the combobox that matches the current value of the item
        QString currentText = index.data(Qt::EditRole).toString();
        int cbIndex = cb->findText(currentText);
        // if it is valid, adjust the combobox
        if (cbIndex >= 0)
            cb->setCurrentIndex(cbIndex);
        } else {
            QStyledItemDelegate::setEditorData(editor, index);
        }
}

void ColorTablleComboBox::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor))
        // save the current text of the combo box as the current value of the item
        model->setData(index, cb->currentText(), Qt::EditRole);
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

void ColorTablleComboBox::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    editor->setGeometry(option.rect);
    (void) index;
}
