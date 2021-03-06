/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_humidity.h"
#include <base/publish.h>
#include "vapour_flux_base.h"

using namespace std;
using namespace base;

namespace vg {
	
PUBLISH(IndoorsHumidity)

/*! \class IndoorsHumidity
 * \brief Computes indoors humidity
 *
 * Inputs
 * ------
 * - _conductance_ is the vapour flux conductance [m/s]
 * - _vapourFlux_ is the vapour flux rate [kg/m<SUP>2</SUP>/s]
 * - _gain_ is used to integrate over all vapour fluxes (GCC, p. 148) [kg/m<SUP>2</SUP>/s]
 * - _temperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _height_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step[s]
 *
 * Outputs
 * ------
 * - _rh_ is the indoors relative humidity [0;100]
 * - _ah_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _ahEq_ is the asymptotic (with time) equilibrium absolute humidity [kg/m<SUP>3</SUP>]
 * - _timeConstant_ is the time constant used to integrate _netVapourFlux_ [s<SUP>-1</SUP>]
 * - _surplusAh_ is an integration inaccuracy that was rounded off [kg/m<SUP>3</SUP>]
 * - _netVapourFlux_ is the total flux of water vapour in/out (+/-) of the greenhouse [kg/m<SUP>2</SUP>/s]
 */

IndoorsHumidity::IndoorsHumidity(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(conductance).imports("indoors/total/vapourFlux[conductance]");
    Input(vapourFlux).imports("indoors/total/vapourFlux[vapourFlux]");
    Input(gain).imports("indoors/total/vapourFlux[gain]");
    Input(temperature).imports("indoors/temperature[value]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Output(rh);
    Output(ah);
    Output(ahEq);
    Output(timeConstant);
    Output(surplusAh);
    Output(netVapourFlux);
}

void IndoorsHumidity::reset() {
    tick = 0;
    rh = 70.;
    ah = ahEq = ahFromRh(temperature, rh);
    netVapourFlux = timeConstant = surplusAh = 0.;
}

void IndoorsHumidity::update() {
    // Keep humidity constant for the first few time steps to stabilise overall model state
    if (tick++ < 10) return;
    double prevAh = ah;
    if (conductance > 0. && gain > 0.) {
        ahEq = gain/conductance;
        timeConstant = height/gain;
        ah = ahEq - (ahEq-ah)*exp(-timeStep/timeConstant);
    }
    else {
        ahEq = ah;
        timeConstant = 0.;
    }

    double indoorsSah = sah(temperature);
    if (ah > indoorsSah) {
        ah = indoorsSah;
        surplusAh = ah - indoorsSah;
    }
    else {
        surplusAh = 0.;
    }

    rh = rhFromAh(temperature, ah);
    netVapourFlux = (ah - prevAh)*height/timeStep; // kg/m2/s = kg/m3 * m3/m2 / s
}

} //namespace
