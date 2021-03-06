/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include <base/publish.h>
#include "co2_controller.h"

using std::min;
using namespace base;

namespace vg {

PUBLISH(Co2Controller)

/*! \class Co2Controller
 * \brief Sets the rate of CO<SUB>2</SUB> enrichment
 * CO<SUB>2</SUB> injection begins, when the level falls below the minimum setpoint,
 * and lasts until the level reaches the maximum setpoint.
 *
 * Inputs
 * ------
 * - _minCo2_ is the minimum setpoint for CO<SUB>2</SUB> in the greenhouse [ppm]
 * - _maxCo2_ is the maximum setpoint for CO<SUB>2</SUB> in the greenhouse [ppm]
 * - _indoorsCo2_ is the indoors CO<SUB>2</SUB> level [ppm]
 * - _timeStep_ is the integration time step [s]
 * - _injectionRate_ is the rate at which CO<SUB>2</SUB> can be injected [g/m<SUP>2</SUP>/h]
 *
 * Output
 * -----
 * - _signal_ is the set rate of CO<SUB>2</SUB> enrichment [g/m<SUP>2</SUP>/h]
 *
 */

Co2Controller::Co2Controller(QString name, QObject *parent)
	: Box(name, parent)
{
    Class(Co2Controller);
    Input(minCo2).imports("setpoints/co2/minimum[signal]");
    Input(maxCo2).imports("setpoints/co2/maximum[signal]");
    Input(indoorsCo2).imports("indoors/co2[value]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(injectionRate).equals(4.5);
    Output(signal);
}

void Co2Controller::reset() {
    signal = 0.;
    on = false;
}

void Co2Controller::update() {
    if (indoorsCo2 > maxCo2)
        on = false;
    else if (indoorsCo2 < minCo2)
        on = true;
    signal = on ? injectionRate : 0.;
}

} //namespace
