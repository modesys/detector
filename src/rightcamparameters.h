#ifndef RIGHTCAMPARAMETERS_H
#define RIGHTCAMPARAMETERS_H
#include <QString>
#include <QVector>

class rightCamParameters
{
public:
    rightCamParameters() = default;
    rightCamParameters(int idR, const QString &nameR, bool saveR,
                      const double latitudeR, const double longitudeR,
                      const double depthR, const double lengthR, const QString &primarysubR,
                      const QString &secondarysubR, const QString &speciesR, const QString &addparamR);


    int idR() const {return rId; }
    bool saveR() const { return rSave; }
    double latitudeR() const { return rLatitude; }
    double longitudeR() const {return rLongitude; }
    double depthR() const { return rDepth; }
    double lengthR() const { return rLength; }
    QString nameR() const { return rName; }
    QString primarysubR() const { return rPrimarySub; }
    QString secondarysubR() const { return rSecondarySub; }
    QString speciesR() const { return rSpecies; }
    QString addparamR() const { return rAddParam; }

private:
    int rId;
    bool rSave;
    double rLatitude;
    double rLongitude;
    double rDepth;
    double rLength;
    QString rName;
    QString rPrimarySub;
    QString rSecondarySub;
    QString rSpecies;
    QString rAddParam;
};

#endif // RIGHTCAMPARAMETERS_H
