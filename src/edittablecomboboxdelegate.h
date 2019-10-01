#ifndef EDITTABLECOMBOBOXDELEGATE_H
#define EDITTABLECOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

class editTableComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    editTableComboBoxDelegate(QObject* parent= nullptr);
    ~editTableComboBoxDelegate();

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                       const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent,
        const QStyleOptionViewItem &/* option */,
        const QModelIndex &/* index */) const;
};

#endif // EDITTABLECOMBOBOXDELEGATE_H
