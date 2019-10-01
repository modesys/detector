#ifndef MGRAPHICSVIEW_H
#define MGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>
#include <QDebug>

class MGraphicsScene : public QGraphicsScene
{
     Q_OBJECT

public:
    explicit MGraphicsScene(QObject *parent = 0);

    // Added on July 12
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

    signals:
    void mouseReleased(QPointF point);
    void rightClick(QPointF point);

    // Added on July 12
    void mouseDoubleClicked(QPointF point);
};

#endif // MGRAPHICSVIEW_H
