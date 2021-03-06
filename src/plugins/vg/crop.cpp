/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(Crop)

/*! \class Crop
 * \brief Crop growth, development and microclimate
 *
 * Inputs
 * ------
 * - _density_ is the number of plants per ground area [m<SUP>-2</SUP>]
 *
 * Outputs
 * -------
 * - none
 */

Crop::Crop(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(density).equals(1);
}


} //namespace

