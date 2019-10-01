#ifndef LEFTCAMPARAMETERS_H
#define LEFTCAMPARAMETERS_H
#include <QString>
#include <QVector>
#include <QPoint>
#include <QDateTime>

typedef struct SelectionData
{
    int mId;
    QByteArray mSave;
    QByteArray mClassImg;
    double mLatitude;
    double mLongitude;
    double mDepth;
    double mLength;
    double mAvgRed;
    double mAvgGreen;
    double mAvgBlue;
    QPoint mACoord;
    QPoint mBCoord;
    QPoint mCCoord;
    QPoint mDCoord;
    QString mName;
    QString mPrimarySub;
    QString mSecondarySub;
    QString mSpecies;
    QString mAddParam;
    QString mPath;
    QDateTime mDateTime;
    double firstXA;
    double firstYA;
    double secondXA;
    double secondYA;

    bool save;

} SelectionData;


//typedef struct rowSelection
//{
//    int numRows;
//    int rowHeight;
//    int xAnchor;
//    int yAnchor;
//} rowSelection;

class leftCamParameters
{
public:
    leftCamParameters();
    leftCamParameters(SelectionData newdata);
    //leftCamParameters(rowSelection newdataRows);
    leftCamParameters(int id, const QString &name, const QString &path, const QDateTime & dateTime,
                      const double firstPointX, const double firstPointY, const double secondPointX, const double secondPointY,
                      const double latitude, const double longitude,
                      const double depth, const double length, const QString &primarysub,
                      const double avgRed, const double avgGreen, const double avgBlue,
                      const QString &secondarysub, QPoint aCoord, QPoint bCoord, QPoint cCoord, QPoint dCoord, const QString &species, const QString &addparam,
                      const QByteArray &save, const QByteArray &mClassImg);


    int id() const { return data.mId; }
    QString path() const { return data.mPath; }
    double latitude() const {return data.mLatitude; }
    double longitude() const { return data.mLongitude; }
    double depth() const { return data.mDepth; }
    double length() const {return data.mLength; }
    double avgRed() const { return data.mAvgRed; }
    double avgGreen() const { return data.mAvgGreen; }
    double avgBlue() const { return data.mAvgBlue; }
    QString name() const {return data.mName; }
    QString primarysub() const { return data.mPrimarySub; }
    QString secondarysub() const { return data.mSecondarySub; }
    QString species() const { return data.mSpecies; }
    QString addparam() const { return data.mAddParam; }
    QPoint mACoord() const { return data.mACoord; }
    QPoint mBCoord() const { return data.mBCoord; }
    QPoint mCCoord() const { return data.mCCoord; }
    QPoint mDCoord() const { return data.mDCoord; }
    QByteArray save() const { return data.mSave; }
    QByteArray classImg() const { return data.mClassImg; }
    QDateTime dateTime() const { return data.mDateTime; }
    double firstPointX() const { return data.firstXA; }
    double firstPointY() const { return data.firstYA; }
    double secondPointX() const { return data.secondXA; }
    double secondPointY() const { return data.secondYA; }

    SelectionData getData() const { return data; }
    void setData(SelectionData ndat) { data = ndat; }

//public:
//    leftCamParameters(int numRows, int rowHeight, int xAnchor,  int yAnchor);

//    int numRowws() const { return dataOfTheRows.numRows;  }
//    int rowHeight() const { return dataOfTheRows.rowHeight; }
//    int xAnchor() const { return dataOfTheRows.xAnchor; }
//    int yAnchor() const { return dataOfTheRows.yAnchor; }

private:
    SelectionData data;
    //rowSelection dataOfTheRows;


};

//QDataStream &operator<<(QDataStream &out, const leftCamParameters &mClipSceneLeft);
//QDataStream &operator>>(QDataStream &in, leftCamParameters &mClipSceneLeft);

#endif // LEFTCAMPARAMETERS_H
