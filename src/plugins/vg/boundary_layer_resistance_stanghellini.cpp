/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance_stanghellini.h"
#include "general.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(BoundaryLayerResistanceStanghellini)

/*! \class BoundaryLayerResistanceJones
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Acording to Stanghellini (1987, p. 32)
 */

BoundaryLayerResistanceStanghellini::BoundaryLayerResistanceStanghellini(QString name, QObject *parent)
    : BoundaryLayerResistanceBase(name, parent)
{
    Class(BoundaryLayerResistanceStanghellini);
    Input(leafTemperature).imports("../temperature[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
}

void BoundaryLayerResistanceStanghellini::setRbH2O() {
    double dTemp = fabs(leafTemperature - indoorsTemperature);
    rbH2O = 1174*sqrt(leafDimension)/pow(leafDimension*dTemp + 207*sqr(leafWindSpeed), 0.25);
}

} //namespace

