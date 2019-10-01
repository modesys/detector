#ifndef COLORTABLLECOMBOBOX_H
#define COLORTABLLECOMBOBOX_H

#include <QStyledItemDelegate>

class ColorTablleComboBox : public QStyledItemDelegate
{
public:
    ColorTablleComboBox(QObject* parent= nullptr);
    ~ColorTablleComboBox();

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                       const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent,
        const QStyleOptionViewItem &/* option */,
        const QModelIndex &/* index */) const;
};

#endif // COLORTABLLECOMBOBOX_H
