/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "pid_control_element.h"
#include <base/publish.h>

using namespace base;

namespace vg {
	
PUBLISH(PidControlElement)

/*! \class PidControlElement
* \brief PID control of a signal
*
*
* Inputs
* ------
* - _Kprop_ is the proportional gain [-]
* - _Kint_ is the integral gain [-]
* - _Kderiv_ is the derivative gain [-]
*
* Outputs
* -------
* - _errorIntegral_ is the integral error [-]
* - _errorDerivative_ is the derivative error [-]
*/

PidControlElement::PidControlElement(QString name, QObject *parent)
    : BaseControlElement(name, parent)
{
    Input(Kprop).equals(0.5);
    Input(Kint).equals(0.);
    Input(Kderiv).equals(0.);
    Output(errorIntegral);
    Output(errorDerivative);
}

void PidControlElement::localReset() {
    errorIntegral = 0.;
    firstChange = true;
}

double PidControlElement::change(double error) {
    errorDerivative = firstChange ? 0. : (error - prevError); // /timeStepSecs;
    prevError = error;
    firstChange = false;
    errorIntegral += error; //*timeStepSecs;
    return Kprop*error + Kint*errorIntegral + Kderiv*errorDerivative;
}

} //namespace

