/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <base/publish.h>
#include "air_flux_cooling_supply_max.h"
#include "general.h"

using std::min;
using std::max;
using namespace base;

namespace vg {

PUBLISH(AirFluxCoolingSupplyMax)

/*! \class AirFluxCoolingSupplyMax
 * \brief Maximum possible air flux from outdoors to greenhouse indoors.
 * This is the maximum air flux that can be effectuated by ventilation.
 * Inputs
 * ------
 * - _byWind_ is the maximum possible air flux that can be caused by outdoors wind alone [h<SUP>-1</SUP>]
 * - _byTemp_ is the maximum possible air flux that can be caused by the outdoors-indoors temperature difference alone [h<SUP>-1</SUP>]
 * - _givenAirFlux_ is the air flux given, irrespective of temperature-regulated ventilation [h<SUP>-1</SUP>]
 *
 * Output
 * ------
 * - _value_ is the maximum possible air flux from outdoors to indoors [h<SUP>-1</SUP>]
 */

AirFluxCoolingSupplyMax::AirFluxCoolingSupplyMax(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(AirFluxCoolingSupplyMax);
    Input(byWind).imports("./byWind[value]");
    Input(byTemp).imports("./byTemp[value]");
    Input(givenAirflux).imports("given/airFlux[value]");
    Output(value);
}

void AirFluxCoolingSupplyMax::reset() {
    value = 0.;
}

void AirFluxCoolingSupplyMax::update() {
    value = max(sqrt(sqr(byWind) + sqr(byTemp)) - givenAirflux, 0.);
}

} //namespace

