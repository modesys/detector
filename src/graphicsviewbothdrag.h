#ifndef GRAPHICSVIEWBOTHDRAG_H
#define GRAPHICSVIEWBOTHDRAG_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>
#include <QDebug>
#include <QGraphicsLineItem>

class MGraphicsViewBothDrag : public QGraphicsView
{
     Q_OBJECT

public:
    explicit MGraphicsViewBothDrag(QObject *parent = 0);

    void setModifiers(const Qt::KeyboardModifiers &modifiers);

    bool gridVisible() const { return _gridVisible; }
    void setGridVisible(bool gridVisible)
    {
      _gridVisible = gridVisible;
      viewport()->update();
    }


public slots:
    //virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void dragMoveEvent(QDragMoveEvent* dragEvent);

    signals:
    void mouseReleased(QPointF point);
    void rightClick(QPointF point);
    void dragMove(QDragMoveEvent* de);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    void applyZoom(double factor, const QPoint &fixedViewPos);
    Qt::KeyboardModifiers m_modifiers;
    const double base = 1.0015;
    double s, scaleFactor = 1.10;

    int _gridSize;
    QColor _gridColor;
    bool _gridVisible;


signals:
   void     scaleSignal(double s, QPointF pos);
   void     wheelSignal(QWheelEvent *event);

public slots:
   void     scaleSlot(double s, QPointF pos);
   void     wheelSlot(QWheelEvent *event);

protected:
  void paintEvent(QPaintEvent *pQEvent) override
  {
      QGraphicsView::paintEvent(pQEvent);

//    QPainter qPainter(viewport());
//    if (_gridVisible) {
//      const int wView = viewport()->width(), hView = viewport()->height();
//      const QPointF offs = mapToScene(0, 0);
//      qPainter.setPen(_gridColor);
//      for (int x = (int)offs.x() % _gridSize; x < wView; x += _gridSize) {
//        qPainter.drawLine(x, 0, x, hView - 1);
//      }
//      for (int y = (int)offs.y() % _gridSize; y < hView; y += _gridSize) {
//        qPainter.drawLine(0, y, wView - 1, y);
//      }
//    }
    //QGraphicsView::paintEvent(pQEvent);
  }



};

#endif // GRAPHICSVIEWBOTHDRAG_H
