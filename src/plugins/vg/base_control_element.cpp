/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <limits>
#include <base/test_num.h>
#include "base_control_element.h"
#include "general.h"
#include "utilities.h"

using namespace base;

namespace vg {

/*! \class BaseControlElement
 * \brief Base class for control elements which maintain a _state_ in response to a _signal_
 *
 * Depending on the logic implemented in the _change_ method, _state_ will approach
 * _signal_ with time
 *
 * Inputs
 * ------
 * - _initState_ is the initial state when the control element is reset [-]
 * - _minimum_ is the minimum value that state can attain [-]
 * - _maximum_ is the maximum value that state can attain [-]
 * - _minSlope_ is the minimum slope that the rate of change in state can attain [min<SUP>-1</<SUP>]
 * - _maxSlope_ is the maximum slope that the rate of change in state can attain [min<SUP>-1</<SUP>]
 * - _timeStep_ is the integration time step [s]
 * - _signal_ is the desired state [-]
 *
 * Outputs
 * ------
 * - _state_ is the current state [-]
 * - _value_ is synonumous with _state_ [-]
 * - _slope_ is the rate of change in state [min<SUP>-1</<SUP>]
 */

BaseControlElement::BaseControlElement(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(BaseControlElement);
    Input(initState);
    Input(minimum).equals(-std::numeric_limits<double>::max());
    Input(maximum).equals(std::numeric_limits<double>::max());
    Input(minSlope).equals(-std::numeric_limits<double>::max());
    Input(maxSlope).equals(std::numeric_limits<double>::max());
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(signal).imports("..[signal]");
    Output(state);
    Output(value);
    Output(slope);
}

void BaseControlElement::reset() {
    state = value = state0 = state1 = state2 = initState;
    slope = 0;
    tick = 0;
    localReset();
}

void BaseControlElement::update() {
    state0 = state1;
    state1 = state2;
    state2 = state;
    state += change(signal - state);
    double dt = timeStep/60.;
    slope = fitSlopePPP(state1, state2, state)/dt;
    if (tick++>10) {
        if (slope < minSlope) {
            state = fitPointPPS(state1, state2, minSlope*dt);
            slope = minSlope;
        }
        else if (slope > maxSlope) {
            state = fitPointPPS(state1, state2, maxSlope*dt);
            slope = maxSlope;
        }
    }
    state = value = minMax(minimum, state, maximum);
}

} //namespace
