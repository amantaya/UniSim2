/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "budget.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(Budget)

/*! \class Budget
 * \brief Greenhouse economy
 *
 * Inputs
 * ------
 * - _heatingEnergyFlux_ is the heating intensity [W/m<SUP>2</SUP>]
 * - _growthLightsEnergyFlux_ is the rate if energy spent on growthLights [W/m<SUP>2</SUP>]
 * - _co2Flux_ is the CO<SUB>2</SUB> enrichment rate [g/m<SUP>2</SUP>/h]
 * - _timeStep_ is the integration time step [s]
 * - _energyUnit_ is the unit use in the output for energy totals ["GJ", "kWh"]
 *
 * Output
 * ------
 * - _heatingEnergyTotal_ is the accumulated energy spent on heating [GJ/m<SUP>2</SUP> or kWh/m<SUP>2</SUP>]
 * - _growthLightsEnergyTotal_ is the accumulated energy spent on heating [GJ/m<SUP>2</SUP> or kWh/m<SUP>2</SUP>]
 * - _co2Total_ is the accumulated CO<SUB>2</SUB> spent [kg/m<SUP>2</SUP>]
 */

Budget::Budget(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(heatingEnergyFlux).imports("heating/supply[value]");
    Input(growthLightsEnergyFlux).imports("actuators/growthLights[energyFlux]");
    Input(co2Flux).imports("controllers/co2[signal]");
    Input(dt).imports("calendar[timeStepSecs]");
    Input(energyUnit).equals("GJ");
    Output(heatingEnergyTotal);
    Output(growthLightsEnergyTotal);
    Output(co2Total);
}

void Budget::reset() {
    heatingEnergyFlux = heatingEnergyTotal =
    growthLightsEnergyFlux = growthLightsEnergyTotal =
    co2Flux = co2Total = 0.;
    QString u = energyUnit.toLower();
    if (u=="gj")
        eUnit = dt*1e-9;          // Gs = s * G
    else if (u=="kwh")
        eUnit = dt/3600.*1e-3;    // kh = s / (s/h) * k
    else {
        QString msg {"Unknown energy unit '%1', expected 'kWh' or 'GJ'"};
        ThrowException(msg.arg(energyUnit)).value(energyUnit).context(this);
    }
}

void Budget::update() {
    heatingEnergyTotal += heatingEnergyFlux*eUnit;
    growthLightsEnergyTotal += growthLightsEnergyFlux*eUnit;
    co2Total += co2Flux*dt/3600.*1e-3;   // g/m2/h * s / (s/h) * k
}


} //namespace

