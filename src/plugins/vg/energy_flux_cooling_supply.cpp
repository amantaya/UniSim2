/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "energy_flux_cooling_supply.h"
#include "general.h"
#include <base/publish.h>

using std::min;
using std::max;
using namespace base;

namespace vg {

PUBLISH(EnergyFluxCoolingSupply)

/*! \class EnergyFluxCoolingSupply
 * \brief Flux of energy supplied to cool the greenhouse down towards the ventilation setpoint
 * Inputs
 * ------
 * - _airSupplyMax_ is the maximum possible air flux from outdoors to indoors [h<SUP>-1</SUP>]
 * - _energyDemand_ is the flux of energy demanded to cool the greenhouse down to the ventilation setpoint [W/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the indoors temperature [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the outdoors temperature [<SUP>o</SUP>C]
 * - _height_ is the average height of the greenhouse [m]
 * - _airTransmissivity_ is the transmissivity of air throught the greenhouse shelter and screens [0;1]
 *
 * Output
 * ------
 * - _value_ is the flux of energy supplied to cool the greenhouse down towards the ventilation setpoint [W/m<SUP>2</SUP>]
 */

EnergyFluxCoolingSupply::EnergyFluxCoolingSupply(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(airSupplyMax).imports("cooling/airSupplyMax[value]");
    Input(energyDemand).imports("cooling/demand[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(airTransmissivity).imports("construction/shelters[airTransmissivity]");
    Output(value);
}

void EnergyFluxCoolingSupply::update() {
    double dT = outdoorsTemperature - indoorsTemperature;
    if (dT < 0.) {
        // W/m2 = m * h-1 / (s/h) * K * J/kg/K * kg/m3
        double energySupplyMax = height*airSupplyMax*airTransmissivity/3600*dT*CpAir*RhoAir;
        value = max(energyDemand, energySupplyMax); // both are negative or zero
    }
    else {
        value = 0.;
    }
}

} //namespace

