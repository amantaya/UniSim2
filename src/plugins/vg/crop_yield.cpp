/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_yield.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(CropYield)

/*! \class CropYield
 * \brief CropYield
 *
 * Inputs
 * ------
 * - _produceMass_ is the accumulated dry mass of the harvestable produce [g/m<SUP>2</SUP> planted area]
 * - _fractionPlantArea_ is the fraction of planted area in the greenhouse [0;1]
 * - _fractionDryWeight_ is the fraction of produce dry weight [0;1]
 *
 * Output
 * ------
 * - _freshWeight_ is the accumulated fresh weight of harvestable produce [kg/m<SUP>2</SUP> greenhouse area]
 * - _dryWeight_ is the accumulated dry weight of harvestable produce [kg/m<SUP>2</SUP> greenhouse area]
 */

CropYield::CropYield(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(produceMass).imports("../mass[fruit]");
    Input(fractionPlantArea).imports("crop/lai[fractionPlantArea]");
    Input(fractionDryWeight).equals(0.03);
    Output(freshWeight);
    Output(dryWeight);
}

void CropYield::reset() {
    freshWeight = dryWeight = 0.;
}

void CropYield::update() {
    dryWeight = produceMass*fractionPlantArea/1000.;
    freshWeight = dryWeight/fractionDryWeight;
}

} //namespace

