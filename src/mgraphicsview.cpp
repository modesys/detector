#include "mgraphicsview.h"

MGraphicsScene::MGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

// Added on July 12
void MGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    mouseDoubleClicked(mouseEvent->scenePos());
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void MGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * me)
{
    //qDebug() << Q_FUNC_INFO << me->scenePos();
    if(me->button() == Qt::LeftButton)
        mouseReleased(me->scenePos());
    else
        rightClick(me->scenePos());

    QGraphicsScene::mouseReleaseEvent(me);
}
