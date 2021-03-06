/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SURFACE_RADIATION_OUTPUTS_H
#define SURFACE_RADIATION_OUTPUTS_H

#include "surface_radiation.h"
#include <base/box.h>

namespace vg {


class SurfaceRadiationOutputs : public base::Box
{
public:
    SurfaceRadiationOutputs(QString name, QObject *parent);
    void resetRadiationOutputs() ;
    void set(const SurfaceRadiation &rad);
    const SurfaceRadiation* surfaceRadiation() const;
private:
    // Outputs
    double lightTransmissivity, directLightTransmissivity, lwTransmissivity,
           incomingLightAbsorptivity, incomingLightReflectivity,
           incomingDirectLightAbsorptivity, incomingDirectLightReflectivity,
           incomingLwAbsorptivity, incomingLwReflectivity,
           outgoingLightAbsorptivity, outgoingLightReflectivity,
           outgoingDirectLightAbsorptivity, outgoingDirectLightReflectivity,
           outgoingLwAbsorptivity, outgoingLwReflectivity;
    // Data
    SurfaceRadiation rad;

};
} //namespace


#endif
