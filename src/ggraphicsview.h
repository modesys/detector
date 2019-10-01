#ifndef GGRAPHICSVIEW_H
#define GGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>
#include <QDebug>

class GGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GGraphicsScene(QObject *parent = 0);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

    signals:
    void mouseReleased(QPointF point);
    void rightClick(QPointF point);
    void mouseDoubleClicked(QPointF point);};

#endif // GGRAPHICSVIEW_H
