#include "dataregionprimarysub.h"

DataRegionPrimarySub::DataRegionPrimarySub(QRectF newarea)
{
    area = newarea;
    identifyMask = IDENT_NONE;
    colorActive = QColor(Qt::red);
    colorInactive = QColor(Qt::darkRed);

    gfx = new QGraphicsItemGroup();

    box = new QGraphicsRectItem(area);
    box->setPen(QPen(colorActive));

    text = new QGraphicsSimpleTextItem("Box!");
    text->setPos(newarea.bottomLeft() + QPointF(0, 2));
    text->setBrush(QBrush(colorActive));
    text->setFont(QFont("Sans", 4, 1));

    gfx->addToGroup(box);
    gfx->addToGroup(text);
}

DataRegionPrimarySub::~DataRegionPrimarySub()
{
    delete box;
    delete text;
    delete gfx;
}

void DataRegionPrimarySub::setColors(QColor newactive, QColor newinactive)
{
    colorActive = newactive;
    colorInactive = newinactive;
}

void DataRegionPrimarySub::setIdentifyMask(int newmask)
{
    identifyMask = newmask;
}

QGraphicsItemGroup* DataRegionPrimarySub::getGraphics()
{
    return gfx;
}

void DataRegionPrimarySub::update()
{
    // Draw all
    QString identifyText = "";

    if(identifyMask & IDENT_NAME) identifyText += name + "\n";
    if(identifyMask & IDENT_PRIMARY) identifyText += primarySub + "\n";
    if(identifyMask & IDENT_SECONDARY) identifyText += secondarySub + "\n";
    if(identifyMask & IDENT_SPECIES) identifyText += species + "\n";
    if(identifyMask & IDENT_ADDPARAM) identifyText += addParam + "\n";

    text->setText(identifyText);
}

void DataRegionPrimarySub::setIdentify(bool newstate)
{
    if(newstate)
        update();
    else
        text->setText("");
}

void DataRegionPrimarySub::setVisible(bool newstate)
{
    box->setVisible(newstate);
    text->setVisible(newstate);
}

void DataRegionPrimarySub::setActive(bool newstate)
{
    if(newstate)
    {
        box->setPen(QPen(colorActive));
        text->setPen(QPen(colorActive));
    }
    else
    {
        box->setPen(QPen(colorInactive));
        text->setPen(QPen(colorInactive));
    }
}

