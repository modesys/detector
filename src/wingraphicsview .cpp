#include "wingraphicsview.h"

WinGraphicsScene::WinGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void WinGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    mouseDoubleClicked(mouseEvent->scenePos());
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void WinGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void WinGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void WinGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * me)
{
    //qDebug() << Q_FUNC_INFO << me->scenePos();
    if(me->button() == Qt::LeftButton)
        mouseReleased(me->scenePos());
    else
        rightClick(me->scenePos());

    QGraphicsScene::mouseReleaseEvent(me);
}
