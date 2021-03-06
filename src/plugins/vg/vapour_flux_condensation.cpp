
/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "vapour_flux_condensation.h"
#include "general.h"
#include <base/publish.h>

using namespace base;
using std::max;

namespace vg {

PUBLISH(VapourFluxCondensation)

/*! \class VapourFluxCondensation
 * \brief Condensation on a surface inside the greenhouse
 *
 * Inputs
 * ------
 * - _surfaceAreaPerGroundArea_ is the area of the surface per greenhouse area [m<SUP>2</SUP>/m<SUP>2</SUP>]
 * - _surfaceTemperature_ is the temperature of the surface [<SUP>o</SUP>C]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _indoorsAh_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 */

VapourFluxCondensation::VapourFluxCondensation(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    Input(surfaceAreaPerGroundArea).equals(0);
    Input(surfaceTemperature).imports("indoors/temperature[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(indoorsAh).imports("indoors/humidity[ah]");
}

void VapourFluxCondensation::update() {
    double dTemp = indoorsTemperature - surfaceTemperature;
    conductance = dTemp > 0
                  ? surfaceAreaPerGroundArea*1.64e-3*pow(dTemp, 1/3.) // GCC, p.147
                  : 0.;
    double surfaceSah = sah(surfaceTemperature);
    vapourFlux = max(conductance*(indoorsAh - surfaceSah), 0.);
    gain = conductance*surfaceSah;
}

} //namespace

