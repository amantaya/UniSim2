/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "outdoors.h"
#include "general.h"
#include <base/publish.h>

using namespace base;

namespace vg {
	
PUBLISH(Outdoors)

/*! \class Outdoors
 * \brief Outdoors weather conditions
 *
 * Inputs (also used as outputs)
 * -----------------------------
 * - _propParRadiation_ is the proportion of PAR in sunlight irradiation [0;1]
 * - _co2_ is the CO<SUB>2</SUB> concentration [ppm]
 * - _temperature_ is the ambient temperature [<SUP>o</SUP>C]
 * - _soilTemperature_ is outdoors soil temperature [<SUP>o</SUP>C]
 * - _rh_ is the relative humidity of the air [0;100]
 * - _radiation_ is total sunlight irradiation (_diffuseRadiation_+_directRadiation_) [W/m<SUP>2</SUP>]
 * - _diffuseRadiation_ is the diffuse component of sunlight irradiation [W/m<SUP>2</SUP>]
 * - _windSpeed_ is the average wind speed [m/s]
 * - _skyTemperature_ is the temperature of the sky [<SUP>o</SUP>C]
 *
 * Outputs
 * -------
 * - _parRadiation_ is the PAR component of sunlight irradiation [W/m<SUP>2</SUP>]
 * - _directRadiation_ is the direct component of sunlight irradiation [W/m<SUP>2</SUP>]
 * - _propDirectRadiation is the proportion of the direct component of the total sunlight [0;1]
 * - _ah_ is the absolute humidity of the air [kg/m<SUP>3</SUP>]
 * - _sh_ is the specific humidity of the air [kg/kg]
 */

Outdoors::Outdoors(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(propParRadiation).equals(0.47);
    Input(co2).equals(400.);
    Input(soilTemperature).equals(10);
    Input(temperature).imports("./records[Tair]");
    Input(rh).imports("./records[Rhair]");
    Input(radiation).imports("./records[GlobRad]");
    Input(diffuseRadiation).imports("./records[DifRad]");
    Input(windSpeed).imports("./records[Windspeed]");
    Input(skyTemperature).imports("./records[Tsky]");
    Output(parRadiation);
    Output(directRadiation);
    Output(propDirectRadiation);
    Output(ah);
    Output(sh);
}

void Outdoors::reset() {
    update();
}

void Outdoors::update() {
    parRadiation = propParRadiation*radiation;
    directRadiation = radiation - diffuseRadiation;
    propDirectRadiation = div0(directRadiation, radiation);
    ah = ahFromRh(temperature, rh);
    sh = shFromRh(temperature, rh);
}

} //namespace
