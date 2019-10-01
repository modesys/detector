#ifndef COORDINATEDELEGATE_H
#define COORDINATEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QVariant>
#include <QPainter>
#include <QString>
#include <QPoint>
#include <QDebug>

class CoordinateDelegate : public QStyledItemDelegate
{
public:
CoordinateDelegate(QObject *parent = 0): QStyledItemDelegate(parent)
{}

void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
if (QVariant::Point == index.data().type())
{
painter->drawText(option.rect,
displayText(index.data(), QLocale::system()),
option.displayAlignment);
}
else
QStyledItemDelegate::paint(painter, option, index);
}

QString displayText(const QVariant & value, const QLocale & locale ) const
{
Q_UNUSED(locale);
qDebug() << "Delegate called with " << value;
return QString("{%1,%2}").arg(value.toPoint().x()).arg(value.toPoint().y() );
}
};

#endif // COORDINATEDELEGATE_H
