#include "rightcamparameters.h"

rightCamParameters::rightCamParameters(int idR, const QString &nameR, bool saveR,
                                        const double latitudeR, const double longitudeR,
                                        const double depthR, const double lengthR, const QString &primarysubR,
                                        const QString &secondarysubR, const QString &speciesR, const QString &addparamR)
{
   rId = idR;
   rSave = saveR;
   rName = nameR;
   rLatitude = latitudeR;
   rLongitude = longitudeR;
   rDepth = depthR;
   rLength = lengthR;
   rPrimarySub = primarysubR;
   rSecondarySub = secondarysubR;
   rSpecies = speciesR;
   rAddParam = addparamR;
}
