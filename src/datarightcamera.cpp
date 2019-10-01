#include "datarightcamera.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>

#define CREATE_RIGHT_TABLE \
    " CREATE TABLE IF NOT EXISTS rightCamTable" \
    " (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL" \
    ", name TEXT NOT NULL" \
    ", latitude DOUBLE NOT NULL" \
    ", longitude DOUBLE NOT NULL" \
    ", depth DOUBLE NOT NULL" \
    ", length DOUBLE NOT NULL" \
    ", primarySubstrate TEXT NOT NULL" \
    ", secondarySubstrate TEXT NOT NULL" \
    ", species TEXT NOT NULL" \
    ", additionalParameters TEXT NOT NULL" \
    ", save TEXT NOT NULL)"

dataRightCamera::dataRightCamera(QObject *parent) : QObject(parent)
{

}

bool dataRightCamera::inizializationDatabaseRightCamera(const QString &nameDataBase)
{
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.setDatabaseName(nameDataBase);
    bool ok = mDatabase.open();
    if(!ok) {
        mError = mDatabase.lastError().text();
    }
    return ok;
}

bool dataRightCamera::configurationDatabaseRightCamera()
{
    QSqlQuery qry;
    bool ok = qry.exec(CREATE_RIGHT_TABLE);
    if(!ok) {
        mError = qry.lastError().text();
    }
    return ok;
}

bool dataRightCamera::addItem(const rightCamParameters *rightCamItem)
{
    QSqlQuery qry;
    qry.prepare("INSERT INTO leftCamTable (name, latitude, longitude, depth, length, primarySubstrate, secondarySubstrate, species, additionalParameters, save) VALUES (?,?,?,?,?,?,?,?,?,?)");
    qry.addBindValue(rightCamItem->nameR());
    qry.addBindValue(rightCamItem->latitudeR());
    qry.addBindValue(rightCamItem->longitudeR());
    qry.addBindValue(rightCamItem->depthR());
    qry.addBindValue(rightCamItem->lengthR());
    qry.addBindValue(rightCamItem->primarysubR());
    qry.addBindValue(rightCamItem->secondarysubR());
    qry.addBindValue(rightCamItem->speciesR());
    qry.addBindValue(rightCamItem->addparamR());
    qry.addBindValue(rightCamItem->saveR());

    bool ok = qry.exec();
    if(!ok) {
        mError = qry.lastError().text();
    }
    return ok;
}

