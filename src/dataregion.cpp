#include "dataregion.h"

DataRegion::DataRegion(QRectF newarea)
{
    area = newarea;

    colorActive = Qt::red;
    colorInactive = Qt::darkRed;

    box = new QGraphicsRectItem(area);
    box->setPen(QPen(colorActive));

    gfx = new QGraphicsItemGroup();

    text = new QGraphicsSimpleTextItem("-------");
    text->setPos(area.bottomLeft() + QPointF(0, 2));
    text->setBrush(QBrush(colorActive));
    text->setFont(QFont("Sans", 6, 4));

    primarysub = "---";
    secondarysub = "---";
    gfx->addToGroup(box);
    gfx->addToGroup(text);
    index = -1;
}

DataRegion::~DataRegion()
{
    delete box;
    delete text;
    delete gfx;
}

QGraphicsItemGroup* DataRegion::getGraphics()
{
    return gfx;
}


void DataRegion::setPrimarySubText(const QString newtext)
{
    primarysub = newtext;
}

void DataRegion::setSecondarySubText(const QString newtext)
{
    secondarysub = newtext;
}

void DataRegion::setCBoxMeasurements(const QString newtext)
{
    CBoxMeasures = newtext;
}

void DataRegion::updateLabelText()
{
    text->setText(primarysub + " :: " + secondarysub);
}

void DataRegion::updateColor(QColor active, QColor inactive)
{
    colorActive = active;
    colorInactive = inactive;
}

void DataRegion::setPen(QPen newpen)
{
    box->setPen(newpen);
}

QGraphicsRectItem*DataRegion::getBox()
{
    return box;
}

QGraphicsSimpleTextItem*DataRegion::getLabel()
{
    return text;
}

void DataRegion::setBoxHidden(bool newstate)
{
    if(!active)
    {
        box->setVisible(newstate);
        text->setVisible(newstate);
    }
}

void DataRegion::setBoxHiddenOverride(bool newstate)
{
    box->setVisible(newstate);
    text->setVisible(newstate);
}

void DataRegion::setActiveState(bool newactive)
{
    active = newactive;
    if(active)
    {
        box->setPen(QPen(colorActive));
        text->setBrush(QBrush(colorActive));
    }
    else
    {
        box->setPen(QPen(colorInactive));
        text->setBrush(QBrush(colorInactive));
    }
}

void DataRegion::setVisible(bool newstate)
{
    box->setVisible(newstate);
    text->setVisible(newstate);
}
