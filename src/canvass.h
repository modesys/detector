#ifndef CANVASS_H
#define CANVASS_H
#include <vector>
#include <QtWidgets>

class Canvass : public QWidget
{
private:
    struct Geo
    {
        QRectF rect;
        QColor color;
        Geo(const QRectF &rect,
            const QColor &color) : rect(rect), color(color) {}
    };
private:
    bool initDone : 1; // true
    std::vector<Geo> scene; // content to render
    QMatrix mat; // view matrix

public:
    Canvass(): QWidget(), initDone(false) { }
    // destructor.
    virtual ~Canvass() override = default;
    // disabled:
    Canvass(const Canvass&) = delete;
    Canvass& operator=(const Canvass&) = delete;
  private:
    // initializes sample geo
    void init()
    {
      if (initDone) return;
      initDone = true;
      // build scene (with NDC i.e. view x/y range: [-1, 1])
      scene.emplace_back(Geo(QRectF(-1.0f, -1.0f, 2.0f, 2.0f), QColor(0x000000u)));
      scene.emplace_back(Geo(QRectF(-0.2f, -0.2f, 0.4f, 0.4f), QColor(0x00ff00u)));
      scene.emplace_back(Geo(QRectF(-0.8f, -0.8f, 0.4f, 0.4f), QColor(0xff0000u)));
      scene.emplace_back(Geo(QRectF(-0.8f, 0.4f, 0.4f, 0.4f), QColor(0x0000ffu)));
      scene.emplace_back(Geo(QRectF(0.4f, 0.4f, 0.4f, 0.4f), QColor(0xff00ffu)));
      scene.emplace_back(Geo(QRectF(0.4f, -0.8f, 0.4f, 0.4f), QColor(0xffff00u)));
      // get initial scaling
      const int wView = width(), hView = height();
      mat.scale(wView / 2, hView / 2);
      mat.translate(1, 1);
    }
  protected:
    virtual void paintEvent(QPaintEvent *pQEvent) override
    {
      init();
      // render
      QPainter qPainter(this);
#if 0 // This scales line width as well:
      qPainter.setMatrix(_mat);
      for (const Geo &geo : _scene) {
        qPainter.setPen(geo.color);
        qPainter.drawRect(geo.rect);
      }
#else // This transforms only coordinates:
      for (const Geo &geo : scene) {
        qPainter.setPen(geo.color);
        QRectF rect(geo.rect.topLeft() * mat, geo.rect.bottomRight() * mat);
        qPainter.drawRect(rect);
      }
#endif // 0
    }
    virtual void wheelEvent(QWheelEvent *pQEvent) override
    {
      //qDebug() << "Wheel Event:"
      //qDebug() << "mouse pos:" << pQEvent->pos();
      // pos() -> virtual canvas
      bool matInvOK = false;
      QMatrix matInv = mat.inverted(&matInvOK);
      if (!matInvOK) {
        qDebug() << "View matrix not invertible!";
        return;
      }
      QPointF posNDC
        = QPointF(pQEvent->pos().x(), pQEvent->pos().y()) * matInv;
      //qDebug() << "mouse pos (NDC):" << posNDC;
      float delta = 1.0f + pQEvent->angleDelta().y() / 1200.0f;
      //qDebug() << "angleDelta:" << pQEvent->angleDelta().y();
      //qDebug() << "scale factor:" << delta;
      mat.translate(posNDC.x(), posNDC.y()); // origin to spot
      mat.scale(delta, delta); // scale
      mat.translate(-posNDC.x(), -posNDC.y()); // spot to origin
      update();
      pQEvent->accept();
    }
};


#endif // CANVASS_H
