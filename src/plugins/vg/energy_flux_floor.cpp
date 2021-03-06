/* Copyright (C) 2013 by Oliver Koerner, AgroTeh [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_floor.h"
#include "general.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(EnergyFluxFloor)

/*! \class EnergyFluxFloor
 * \brief Greenhouse floor temperature and energy flux to greenhouse
 * Inputs
 * ------
 * - _Uindoors_ is the U-value between greenhouse air and floor [W/m<SUP>2</SUP>]/K]
 * - _Usoil_ is the U-value between outdoors soil and floor [W/m<SUP>2</SUP>]/K]
 * - _heatCapacity_ is the heat capacity of the floor [J/m<SUP>2</SUP>]/K]
 * - _emissivity_ is the emissivity of thermal radiation from the floor [0;1]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>oC</SUP>C]
 * - _soilTemperature_ is the outdoors soil temperature  [<SUP>oC</SUP>C]
 * - _height_ is the average height of the greenhouse (volume divided by ground area) [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the energy flux per greenhouse area from the floor [W/m<SUP>2</SUP>]
 * - _temperature_ is the floor temperature [<SUP>oC</SUP>C]
 */

EnergyFluxFloor::EnergyFluxFloor(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(Uindoors).equals(7.5);
    Input(Usoil).equals(7.5);
    Input(heatCapacity).equals(42000.);
    Input(emissivity).equals(0.85);    // concrete
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(soilTemperature).imports("outdoors[soilTemperature]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(temperature);
}

void EnergyFluxFloor::reset() {
    temperature = 18.;
}

void EnergyFluxFloor::update() {
    const int maxTimeStep = 20;     // Use time steps no larger than this [s]
    int n = int(timeStep/maxTimeStep) + 1;
    double dt = timeStep/n,
           Tin = indoorsTemperature,
           Cair = height*RhoAir*CpAir;               // J/m2/K = m * kg/m3 * J/kg/K

    value = 0.;
    for (int i=0; i<n; ++i) {
        double fluxFloorToSoil = Usoil*(temperature-soilTemperature),
               fluxFloorToAir = Uindoors*(temperature-Tin);
        temperature -= (fluxFloorToSoil + fluxFloorToAir)*dt/heatCapacity;
        Tin += fluxFloorToAir*dt/Cair;
        value += fluxFloorToAir;
    }
    value /= n;
}

} //namespace

