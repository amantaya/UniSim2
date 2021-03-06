/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LEAF_RADIATION_ABSORBED_H
#define VG_LEAF_RADIATION_ABSORBED_H

#include <base/box.h>

namespace vg {

class LeafRadiationAbsorbed : public base::Box
{
public:
    LeafRadiationAbsorbed(QString name, QObject *parent);
    void initialize();
    void update();
private:
    // Inputs
    double
        xGaussLowerside, wGaussLowerside,
        xGaussUpperside, wGaussUpperside,
        kLw, lai, indoorsLight,
        lightAbsorptivity, emissivity,
        growthLightLw, growthLightViewFactor,
        floorTemperature, floorEmissivity,
        leafTemperature, coverTemperature, screensTemperature, screensMaxState,
        shelterOutgoingLwAbsorptivity,
        coverPerGroundArea;

    // Outputs
    double value, lightAbsorbed, heatingAbsorbed,
        growthLightLwAbsorbed,
        floorLwAbsorbed,
        shelterLoss;

    // Data
    double lwTransmissionLowerside, lwTransmissionUpperside;

    struct PipeInfo {
        const double *length, *diameter, *temperature, *emissivity;
        double area() {return (*length)*(*diameter)/1000./2; }  // Only half of the area faces upwards
    };
    QVector<PipeInfo> pipeInfos;

    // Methods
    void setLightAbsorbed();
    void setGrowthLightLwAbsorbed();
    void setFloorLwAbsorbed();
    void setShelterLoss();
    void setHeatingAbsorbed();
};

} //namespace

#endif
