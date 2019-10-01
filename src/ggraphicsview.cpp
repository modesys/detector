#include "ggraphicsview.h"

GGraphicsScene::GGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

// Added on July 12
void GGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    mouseDoubleClicked(mouseEvent->scenePos());
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * me)
{
    //qDebug() << Q_FUNC_INFO << me->scenePos();
    if(me->button() == Qt::LeftButton)
        mouseReleased(me->scenePos());
    else
        rightClick(me->scenePos());

    QGraphicsScene::mouseReleaseEvent(me);
}
