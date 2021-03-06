/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_condensation.h"
#include "general.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(EnergyFluxCondensation)

/*! \class EnergyFluxCondensation
 * \brief Flux of energy produced by condensation
 *
 * Input
 * ------
 * - _vapourFlux_ is the condensation rate per greenhouse ground area [kg/m<SUP>2</SUP>/s]
 */

EnergyFluxCondensation::EnergyFluxCondensation(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(vapourFlux).equals(0);
}

void EnergyFluxCondensation::update() {
    value = vapourFlux*LHe; // W/m2 = kg/m2/s * J/kg
}

} //namespace

