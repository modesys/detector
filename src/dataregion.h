#ifndef DATAREGION_H
#define DATAREGION_H

#include <QObject>
#include <QRect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QPen>
#include <QModelIndex>


class DataRegion : public QObject
{
   Q_OBJECT

public:
    DataRegion(QRectF newarea);
    ~DataRegion();
    void setPen(QPen newpen);
    QGraphicsRectItem *getBox();
    QGraphicsSimpleTextItem *getLabel();
    void updateLabelText();
    void updateColor(QColor active, QColor inactive);

    void setPrimarySubText(const QString newtext);
    void setSecondarySubText(const QString newtext);
    void setCBoxMeasurements(const QString newtext);

    QGraphicsItemGroup* getGraphics();


public slots:
    void setBoxHidden(bool newstate);
    void setBoxHiddenOverride(bool newstate);
    void setActiveState(bool newactive);
    void setVisible(bool newstate);

private:
    QRectF area;
    QGraphicsRectItem *box;
    QGraphicsSimpleTextItem* text;
    QString primarysub;
    QString secondarysub;
    QString CBoxMeasures;
    QColor colorActive, colorInactive;
    bool active;
    QGraphicsItemGroup* gfx;

public:
    int index;

};

#endif // DATAREGION_H
