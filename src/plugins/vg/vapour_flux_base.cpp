/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "vapour_flux_base.h"

using namespace base;

namespace vg {

/*! \class VapourFluxBase
 * \brief Base class for transpiration, evaporation, condensation and ventilation vapour fluxes
 *
 *
 * Outputs
 * ------
 * - _conductance_ is the vapour flux conductance [m/s]
 * - _vapourFlux_ is the vapour flux rate [kg/m<SUP>2</SUP>/s]
 * - _gain_ is used to integrate over all water vapour fluxes (GCC, p. 148) [kg/m<SUP>2</SUP>/s]
 *
 */

VapourFluxBase::VapourFluxBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(VapourFluxBase);
    Output(conductance);
    Output(gain);
    Output(vapourFlux);
}

} //namespace

