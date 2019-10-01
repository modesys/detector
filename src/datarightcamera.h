#ifndef DATARIGHTCAMERA_H
#define DATARIGHTCAMERA_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include "rightcamparameters.h"

class dataRightCamera : public QObject
{
    Q_OBJECT
public:
    explicit dataRightCamera(QObject *parent = nullptr);
    QString getError() const { return mError; }
    bool inizializationDatabaseRightCamera(const QString &nameDataBase);
    bool configurationDatabaseRightCamera();
    bool addItem(const rightCamParameters* rightCamItem);

private:
    QString mError;
    QSqlDatabase mDatabase;

};

#endif // DATARIGHTCAMERA_H
