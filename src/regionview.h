#ifndef REGIONVIEW_H
#define REGIONVIEW_H

#include <QObject>

class RegionView : public QObject
{
    Q_OBJECT
public:
    explicit RegionView(QObject *parent = nullptr);

signals:

public slots:
};

#endif // REGIONVIEW_H