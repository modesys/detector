#ifndef DATALEFTCAMERA_H
#define DATALEFTCAMERA_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include "leftcamparameters.h"

class dataLeftCamera : public QObject
{
    Q_OBJECT
public:
    explicit dataLeftCamera(QObject *parent = nullptr);
    QString getError() const { return mError; }
    bool inizializationDatabaseLeftCamera(const QString &nameDataBase);
    bool configurationDatabaseLeftCamera();
    bool addItem(leftCamParameters* leftCamItem);
    bool updateItem(int itemId,leftCamParameters* leftCamItem);
    bool removeItem(int itemId);
    QSqlDatabase getDatabase();

private:
    QString mError;
    QSqlDatabase mDatabaseLeft;

};

#endif // DATALEFTCAMERA_H
