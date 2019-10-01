#include "graphicsviewbothdrag.h"
#include <QWheelEvent>
#include <QtMath>

MGraphicsViewBothDrag::MGraphicsViewBothDrag(QObject *parent) :
    QGraphicsView()
{
    //setTransformationAnchor(QGraphicsView::AnchorViewCenter);
//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MGraphicsViewBothDrag::setModifiers(const Qt::KeyboardModifiers &modifiers)
{
    m_modifiers = modifiers;
}

void MGraphicsViewBothDrag::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() == m_modifiers){
        double angle = event->orientation() == Qt::Vertical ? event->angleDelta().y(): event->angleDelta().x();
        double factor = qPow(base, angle);
        applyZoom(factor, event->pos());
    }
    else
    {
        if(event->delta() > 0)
        {
            s = scaleFactor;
        }
        else
        {
            s = 1.0/scaleFactor;
        }
        scale(s, s);
    #if 0
        centerOn(event->pos());
    #else
        // CenterOn(mapToScene(event->pos()));
    #endif
        emit scaleSignal(s, event->pos());
        event->accept();
    }
}

void MGraphicsViewBothDrag::applyZoom(double factor, const QPoint &fixedViewPos)
{
    QPointF fixedScenePos = mapToScene(fixedViewPos);
    centerOn(fixedScenePos);
    scale(factor, factor);
    QPointF delta = mapToScene(fixedViewPos) - mapToScene(viewport()->rect().center());
    centerOn(fixedScenePos - delta);
}


void MGraphicsViewBothDrag::scaleSlot(double s, QPointF pos)  {
    int     w = this->width(), h = this->height();
    scale(s, s);
}

void MGraphicsViewBothDrag::wheelSlot(QWheelEvent *event) {
    // setFocus();
    wheelEvent(event);
}


void MGraphicsViewBothDrag::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
}

void MGraphicsViewBothDrag::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
}

void MGraphicsViewBothDrag::mouseReleaseEvent(QGraphicsSceneMouseEvent * me)
{
    //qDebug() << Q_FUNC_INFO << me->scenePos();
    if(me->button() == Qt::LeftButton)
        mouseReleased(me->scenePos());
    else
        rightClick(me->scenePos());
}

void MGraphicsViewBothDrag::dragMoveEvent(QDragMoveEvent *dragEvent)
{
    dragMove(dragEvent);
    QGraphicsView::dragMoveEvent(dragEvent);
}
