#ifndef DATAREGIONPRIMARYSUB_H
#define DATAREGIONPRIMARYSUB_H

#include <QObject>
#include <QRect>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QPen>

enum IdentifyData
{
    IDENT_NAME      = 0x01,
    IDENT_PRIMARY   = 0x02,
    IDENT_SECONDARY = 0x04,
    IDENT_SPECIES   = 0x08,
    IDENT_ADDPARAM  = 0x10,
    IDENT_NPS       = 0x07,
    IDENT_ALL       = 0x1F,
    IDENT_NONE      = 0x00
};

class DataRegionPrimarySub : public QObject
{
    Q_OBJECT

    public:
    DataRegionPrimarySub(QRectF newarea);
    ~DataRegionPrimarySub();

    void setColors(QColor newactive, QColor newinactive);
    void setIdentifyMask(int newmask);

    QGraphicsItemGroup* getGraphics();

    public slots:
    void update();

    void setIdentify(bool newstate);
    void setVisible(bool newstate);
    void setActive(bool newstate);

    public:
    // Data - no getters/setters here
    double latitude;
    double longitude;
    double depth;
    double length;
    double avgRed;
    double avgGreen;
    double avgBlue;
    QString name;
    QString primarySub;
    QString secondarySub;
    QString species;
    QString addParam;

    private:
    // Graphics
    QRectF area;
    QGraphicsRectItem* box;
    QGraphicsSimpleTextItem* text;
    QGraphicsItemGroup* gfx;
    int identifyMask;
    QColor colorActive, colorInactive;
};

#endif // DATAREGIONPRIMARYSUB_H
