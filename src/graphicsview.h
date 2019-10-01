#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent=nullptr);
    void setModifiers(const Qt::KeyboardModifiers &modifiers);

protected:
    void wheelEvent(QWheelEvent *event) override;
private:
    void applyZoom(double factor, const QPoint &fixedViewPos);
    Qt::KeyboardModifiers m_modifiers;
    const double base = 1.0015;

signals:
   void     scaleSignal(double s, QPointF pos);


public slots:
   void     scaleSlot(double s, QPointF pos);
   void     wheelSlot(QWheelEvent *event);

};

#endif // GRAPHICSVIEW_H
