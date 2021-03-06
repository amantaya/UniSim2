/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <base/publish.h>
#include "air_flux_gravitation.h"
#include "general.h"

using namespace base;
using std::min;

namespace vg {

PUBLISH(AirFluxGravitation)

/*! \class AirFluxGravitation
 * \brief Vertical air movement by gravitation through screen gap
 * Inflowing air is assumed to have the qualities of outdoors air.
 *
 * Inputs
 * ------
 * - _screensState_ is the maximum screen state [0;1]
 * - _indoorsTemperature_ is the indoors tempeature [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is ambient temperature outdoors [<SUP>o</SUP>C]
 * - _indoorsVolume_ is the greenhouse volume, reduced when horizontal screen is drawn [m<SUP>3</SUP>]
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the relative rate of air exchanged [h<SUP>-1</SUP>]
 */

AirFluxGravitation::AirFluxGravitation(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(AirFluxGravitation);
    Input(screensState).imports("construction/shelters[screensMaxState]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(indoorsVolume).imports("geometry[indoorsVolume]");
    Input(groundArea).imports("geometry[groundArea]");
    Output(value);
}

void AirFluxGravitation::reset() {
    value = 0.;
}

void AirFluxGravitation::update() {
    double gapArea = (1. - screensState)*groundArea,
           topRho = rhoAir(outdoorsTemperature),
           bottomRho = rhoAir(indoorsTemperature),
           flowRate = (indoorsTemperature > outdoorsTemperature) ?  // m3/h
                      0.06*g*pow(gapArea, 1.5)*sqrt((topRho - bottomRho)/topRho)*screensState*2 : 0.;

    value = flowRate/indoorsVolume;
}

} //namespace

