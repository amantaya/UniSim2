/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_COOLING_SUPPLY_H
#define VG_ENERGY_FLUX_COOLING_SUPPLY_H

#include <base/box.h>

namespace vg {

class EnergyFluxCoolingSupply : public base::Box
{
public:
    EnergyFluxCoolingSupply(QString name, QObject *parent);
    void update();

private:
    // Inputs
    double airSupplyMax, energyDemand,
        indoorsTemperature, outdoorsTemperature, height, airTransmissivity;
    // Output
    double value;
};
} //namespace


#endif
