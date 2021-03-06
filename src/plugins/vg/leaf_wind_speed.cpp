/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "leaf_wind_speed.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(LeafWindSpeed)

/*! \class LeafWindSpeed
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _k_ is the extinction coefficient for wind down throught the canopy [-]
 * - _indoorsWindSpeedMinimum_ is the assumed minimum limit to wind speed indoors [m/s]
 * - _indoorsWindSpeed_ is the wind speed indoors [m/s]
 * - _xGauss_ is the coefficient for Gaussian integration from the upper side of the canopy [0;1]
 * - _lai_ is the crop leaf area index [m<SUP>2</SUP>/m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _value_ is the wind speed at leaf level [m/s]
 */

LeafWindSpeed::LeafWindSpeed(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(k).equals(0.);
    Input(indoorsWindSpeedMinimum).equals(0.025);
    Input(indoorsWindSpeed).imports("indoors/windSpeed[value]");
    Input(xGauss).imports("..[xGaussUpperside]");
    Input(lai).imports("crop/lai[value]");
    Output(value);
}

void LeafWindSpeed::reset() {
    value = indoorsWindSpeedMinimum;
}

void LeafWindSpeed::update() {
    value = max(indoorsWindSpeed, indoorsWindSpeedMinimum)*exp(-k*lai*xGauss);
}


} //namespace

