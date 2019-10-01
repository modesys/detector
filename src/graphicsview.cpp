#include "graphicsview.h"

#include <QWheelEvent>
#include <QtMath>

GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent)
{}

void GraphicsView::setModifiers(const Qt::KeyboardModifiers &modifiers)
{
    m_modifiers = modifiers;
}

//void GraphicsView::wheelEvent(QWheelEvent *event)
//{
//    if(event->modifiers() == m_modifiers)
//    {
//        double angle = event->orientation() == Qt::Vertical ? event->angleDelta().y(): event->angleDelta().x();
//        double factor = qPow(base, angle);
//        applyZoom(factor, event->pos());
//    }
//}

void GraphicsView::applyZoom(double factor, const QPoint & fixedViewPos)
{
    QPointF fixedScenePos = mapToScene(fixedViewPos);
    centerOn(fixedScenePos);
    scale(factor, factor);
    QPointF delta = mapToScene(fixedViewPos) - mapToScene(viewport()->rect().center());
    centerOn(fixedScenePos - delta);
}


void GraphicsView::wheelEvent(QWheelEvent *event)
{

    if(event->modifiers() == m_modifiers)
    {
        double angle = event->orientation() == Qt::Vertical ? event->angleDelta().y(): event->angleDelta().x();
        double factor = qPow(base, angle);
        applyZoom(factor, event->pos());
    }
//    else
//    {
//        double s, scaleFactor = 1.15;

//        if(event->delta() > 0)
//        {
//            s = scaleFactor;
//        }
//        else
//        {
//            s = 1.0/scaleFactor;
//        }


//    scale(s, s);
//#if 0
//    centerOn(event->pos());
//#else
//    // CenterOn(mapToScene(event->pos()));
//#endif

//    emit scaleSignal(s, event->pos());

//    event->accept();

//    // Don't call superclass handler here
//    // as wheel is normally used for moving scrollbars
//    // QGraphicsView::wheelEvent(event);
//}
}

void GraphicsView::scaleSlot(double s, QPointF pos)  {
    int     w = this->width(), h = this->height();

    scale(s, s);
    // CenterOn(pos);
}

void GraphicsView::wheelSlot(QWheelEvent *event) {
    // setFocus();
    wheelEvent(event);
}
