#ifndef WINGRAPHICSVIEW_H
#define WINGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>
#include <QDebug>
#include <QGraphicsView>

class WinGraphicsScene : public QGraphicsScene
{
     Q_OBJECT

public:
    explicit WinGraphicsScene(QObject *parent = 0);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

    signals:
    void mouseReleased(QPointF point);
    void rightClick(QPointF point);
    void mouseDoubleClicked(QPointF point);
};

#endif // WINGRAPHICSVIEW_H
