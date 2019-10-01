#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QAbstractItemDelegate>
#include <QSize>
#include <QPixmap>
#include <QPainter>
#include <QStylePainter>
#include <QStyledItemDelegate>
#include <QDebug>

class ImageDelegate : public QStyledItemDelegate
{
public:
    ImageDelegate(QObject * parent = nullptr);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    QString mdisplayText(const QVariant & value, const QLocale & locale ) const
    {
        qDebug() << "Delegate called with " << value;
        return QString("{%1,%2}").arg(value.toPoint().x()).arg(value.toPoint().y() );
    }
};

#endif // IMAGEDELEGATE_H
