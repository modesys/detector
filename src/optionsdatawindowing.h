#ifndef OPTIONSDATAWINDOWING_H
#define OPTIONSDATAWINDOWING_H

#include <QObject>
#include <QRect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QPen>


class optionsdatawindowing : public QObject
{
   Q_OBJECT

public:
    optionsdatawindowing(QRectF newarea);
    ~optionsdatawindowing();
   

public slots:
 //   void setBoxHidden(bool newstate);
 //   void setBoxHiddenOverride(bool newstate);
 //   void setActiveState(bool newactive);
 //   void setVisible(bool newstate);

private:
    QRectF area;
};

#endif // OPTIONSDATAWINDOWING_H
