/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_temperature.h"
#include <base/publish.h>

using namespace base;

namespace vg {
	
PUBLISH(IndoorsTemperature)

/*! \class IndoorsTemperature
 * \brief Indoors ambient temperature
 *
 * Inputs
 * ------
 * - _resetValue_ is the temperature at time zero [<SUP>o</SUP>C]
 * - _energyFlux_ is the energy flux dissipated into greenhouse air [W/m<SUP>2</SUP>]
 * - _height_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the indoors tempeature [<SUP>o</SUP>C]
 */

IndoorsTemperature::IndoorsTemperature(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(resetValue).equals(20.);
    Input(energyFlux).equals(0.);
    Input(baseTemperature).imports(".[value]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(value);
}


void IndoorsTemperature::reset() {
    value = resetValue;
    tick = 0;
}

void IndoorsTemperature::update() {
    // Keep temperature constant for the first few time steps to stabilise overall model state
    if (tick++ < 10) return;
    double Cair = height*RhoAir*CpAir;               // J/m2/K = m * kg/m3 * J/kg/K
    value = baseTemperature + energyFlux*timeStep/Cair;  // K = W/m2 * s / (J/m2/K)
}

} //namespace
