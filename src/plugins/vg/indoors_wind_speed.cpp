/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "indoors_wind_speed.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(IndoorsWindSpeed)

/*! \class IndoorsWindSpeed
 * \brief Wind speed inside greenhouse
 *
 * Inputs
 * ------
 * - _ventilation_ is the rate of air exchange through ventilation [h<SUP>-1</SUP>]
 * - _width_ is the width of the greenhouse [m]
 * *
 * Outputs
 * ------
 * - _value_ is the wind speed indoors [m/s]
 */

IndoorsWindSpeed::IndoorsWindSpeed(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(ventilation).imports("indoors/total/airFlux[value]");
    Input(constructionWidth).imports("geometry[width]");
    Output(value);
}

void IndoorsWindSpeed::reset() {
    value = 0.;
}

void IndoorsWindSpeed::update() {
    value = ventilation*constructionWidth/3600.; // m/s = h-1 * m *  h/s
    // volume/(length*height) = width
    // See Wang et al. (2000)
}

} //namespace

