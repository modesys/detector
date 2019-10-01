#include "dataleftcamera.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QVariant>

#define CREATE_LEFT_TABLE \
    " CREATE TABLE IF NOT EXISTS leftCamTable" \
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
    ", ACoord TEXT NOT NULL" \
    ", BCoord TEXT NOT NULL" \
    ", CCoord TEXT NOT NULL" \
    ", DCoord TEXT NOT NULL" \
    ", classImg BLOB " \
    ", avgRed DOUBLE NOT NULL" \
    ", avgGreen DOUBLE NOT NULL" \
    ", avgBlue DOUBLE NOT NULL" \
    ", save BLOB NOT NULL" \
    ", path TEXT NOT NULL" \
    ", dateTime DATETIME NOT NULL " \
    ", firstPointX DOUBLE NOT NULL " \
    ", firstPointY DOUBLE NOT NULL " \
    ", secondPointX DOUBLE NOT NULL " \
    ", secondPointY DOUBLE NOT NULL)"

dataLeftCamera::dataLeftCamera(QObject *parent) : QObject(parent)
{

}

bool dataLeftCamera::inizializationDatabaseLeftCamera(const QString &nameDataBase) {
    mDatabaseLeft = QSqlDatabase::addDatabase("QSQLITE");
    mDatabaseLeft.setDatabaseName(nameDataBase);
    bool ok = mDatabaseLeft.open();
    if(!ok) {
        mError = mDatabaseLeft.lastError().text();
    }
    return ok;
}

bool dataLeftCamera::configurationDatabaseLeftCamera() {
    QSqlQuery qry;
    bool ok = qry.exec(CREATE_LEFT_TABLE);
    if(!ok) {
        mError = qry.lastError().text();
    }
    return ok;
}

bool dataLeftCamera::addItem(leftCamParameters *leftCamItem)
{
    QSqlQuery qry;
    qry.prepare("INSERT INTO leftCamTable (name, latitude, longitude," \
                " depth, length, primarySubstrate, secondarySubstrate," \
                " species, additionalParameters, avgRed, avgGreen," \
                " avgBlue, save, path, ACoord, BCoord, CCoord, DCoord,classImg, dateTime, firstPointX, firstPointY, secondPointX, secondPointY)"\
                " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    qry.addBindValue(leftCamItem->name());
    qry.addBindValue(leftCamItem->latitude());
    qry.addBindValue(leftCamItem->longitude());
    qry.addBindValue(leftCamItem->depth());
    qry.addBindValue(leftCamItem->length());
    qry.addBindValue(leftCamItem->primarysub());
    qry.addBindValue(leftCamItem->secondarysub());
    qry.addBindValue(leftCamItem->species());
    qry.addBindValue(leftCamItem->addparam());
    qry.addBindValue(leftCamItem->avgRed());
    qry.addBindValue(leftCamItem->avgGreen());
    qry.addBindValue(leftCamItem->avgBlue());
    qry.addBindValue(leftCamItem->save());
    qry.addBindValue(leftCamItem->path());
    qry.addBindValue(QString::number(leftCamItem->mACoord().x()) + ":" + QString::number(leftCamItem->mACoord().y()));
    qry.addBindValue(QString::number(leftCamItem->mBCoord().x()) + ":" + QString::number(leftCamItem->mBCoord().y()));
    qry.addBindValue(QString::number(leftCamItem->mCCoord().x()) + ":" + QString::number(leftCamItem->mCCoord().y()));
    qry.addBindValue(QString::number(leftCamItem->mDCoord().x()) + ":" + QString::number(leftCamItem->mDCoord().y()));
    qry.addBindValue(leftCamItem->classImg());
    qry.addBindValue(leftCamItem->dateTime());
    qry.addBindValue(leftCamItem->firstPointX());
    qry.addBindValue(leftCamItem->firstPointY());
    qry.addBindValue(leftCamItem->secondPointX());
    qry.addBindValue(leftCamItem->secondPointY());

    bool ok = qry.exec();
    if(!ok) {
        mError = qry.lastError().text();
        qDebug() << mError;
    }
    return ok;
}

bool dataLeftCamera::updateItem(int itemId, leftCamParameters *leftCamItem)
{
    QSqlQuery qry;
    qry.prepare(" UPDATE leftCamTable SET " \
                " name = ?, latitude = ?, longitude = ?," \
                " depth = ?, length = ?, primarySubstrate = ?, secondarySubstrate = ?," \
                " species = ?, additionalParameters = ?, avgRed = ?, avgGreen = ?," \
                " avgBlue = ?, save = ? , path = ?, ACoord = ?," \
                " BCoord = ?, CCoord = ?, DCoord = ?,classImg = ?, dateTime = ?," \
                " firstPointX = ?, firstPointY = ?, secondPointX = ?, secondPointY = ?" \
                " WHERE id = ?");

    qry.addBindValue(leftCamItem->name());
    qry.addBindValue(leftCamItem->latitude());
    qry.addBindValue(leftCamItem->longitude());
    qry.addBindValue(leftCamItem->depth());
    qry.addBindValue(leftCamItem->length());
    qry.addBindValue(leftCamItem->primarysub());
    qry.addBindValue(leftCamItem->secondarysub());
    qry.addBindValue(leftCamItem->species());
    qry.addBindValue(leftCamItem->addparam());
    qry.addBindValue(leftCamItem->avgRed());
    qry.addBindValue(leftCamItem->avgGreen());
    qry.addBindValue(leftCamItem->avgBlue());
    qry.addBindValue(leftCamItem->save());
    qry.addBindValue(leftCamItem->path());
    qry.addBindValue(QString::number(leftCamItem->mACoord().x()) + ":" + QString::number(leftCamItem->mACoord().y()));
    qry.addBindValue(QString::number(leftCamItem->mBCoord().x()) + ":" + QString::number(leftCamItem->mBCoord().y()));
    qry.addBindValue(QString::number(leftCamItem->mCCoord().x()) + ":" + QString::number(leftCamItem->mCCoord().y()));
    qry.addBindValue(QString::number(leftCamItem->mDCoord().x()) + ":" + QString::number(leftCamItem->mDCoord().y()));
    qry.addBindValue(leftCamItem->classImg());
    qry.addBindValue(leftCamItem->dateTime());
    qry.addBindValue(leftCamItem->firstPointX());
    qry.addBindValue(leftCamItem->firstPointY());
    qry.addBindValue(leftCamItem->secondPointX());
    qry.addBindValue(leftCamItem->secondPointY());
    qry.addBindValue(itemId);

    bool ok = qry.exec();
    if(!ok) {
        mError = qry.lastError().text();
        qDebug() << mError;
    }
    return ok;
}

bool dataLeftCamera::removeItem(int itemId)
{
    QSqlQuery qry;
    qry.prepare("DELETE FROM leftCamTable WHERE id = ?");
    qry.addBindValue(itemId);

    bool ok = qry.exec();
    if(!ok) {
        mError = qry.lastError().text();
        qDebug() << mError;
    }
}

QSqlDatabase dataLeftCamera::getDatabase()
{
    return mDatabaseLeft;
}



