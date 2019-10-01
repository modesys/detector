#include "leftcamparameters.h"

leftCamParameters::leftCamParameters(SelectionData newdata)
{
    data = newdata;
}

//leftCamParameters::leftCamParameters(rowSelection newdataRows)
//{
//    dataOfTheRows = newdataRows;
//}


leftCamParameters::leftCamParameters(int id, const QString &name, const QString &path, const QDateTime &dateTime,
                                     const double firstPointX, const double firstPointY, const double secondPointX, const double secondPointY,
                                     const double latitude, const double longitude, const double depth,
                                     const double length, const QString &primarysub, const double avgRed,
                                     const double avgGreen, const double avgBlue, const QString &secondarysub,
                                     QPoint aCoord, QPoint bCoord, QPoint cCoord, QPoint dCoord, const QString &species,
                                     const QString &addparam, const QByteArray &save, const QByteArray &mClassImg)
{
    data.mId = id;
    data.mSave = save;
    data.mPath = path;
    data.mClassImg = mClassImg;
    data.mName = name;
    data.mLatitude = latitude;
    data.mLongitude = longitude;
    data.mDepth = depth;
    data.mLength = length;
    data.mPrimarySub = primarysub;
    data.mSecondarySub = secondarysub;
    data.mSpecies = species;
    data.mAddParam = addparam;
    data.mACoord = aCoord;
    data.mBCoord = bCoord;
    data.mCCoord = cCoord;
    data.mDCoord = dCoord;
    data.mAvgRed = avgRed;
    data.mAvgGreen = avgGreen;
    data.mAvgBlue = avgBlue;
    data.mDateTime = dateTime;
    data.firstXA = firstPointX;
    data.firstYA = firstPointY;
    data.secondXA = secondPointX;
    data.secondYA = secondPointY;
}

leftCamParameters::leftCamParameters()
{
    data.mId = 0;
    data.mSave = QByteArray(5, '1');
    data.mPath =  "";
    data.mClassImg = QByteArray(5, '1');
    data.mName = "";
    data.mLatitude = 0.0;
    data.mLongitude = 0.0;
    data.mDepth = 0.0;
    data.mLength = 0.0;
    data.mPrimarySub = "";
    data.mSecondarySub = "";
    data.mSpecies = "";
    data.mAddParam = "";
    data.mACoord = QPoint(0, 0);
    data.mBCoord = QPoint(0, 0);
    data.mCCoord = QPoint(0, 0);
    data.mDCoord = QPoint(0, 0);
    data.mAvgRed = 0.0;
    data.mAvgGreen = 0.0;
    data.mAvgBlue = 0.0;
    data.mDateTime = QDateTime(QDate(2013, 22, 4), QTime(14, 0, 0));
    data.firstXA = 0.0;
    data.firstYA = 0.0;
    data.secondXA = 0.0;
    data.secondYA = 0.0;
}


//leftCamParameters::leftCamParameters(int numRows, int rowHeight, int xAnchor, int yAnchor)
//{
//    dataOfTheRows.numRows = numRows;
//    dataOfTheRows.rowHeight = rowHeight;
//    dataOfTheRows.xAnchor = xAnchor;
//    dataOfTheRows.yAnchor = yAnchor;
//}

