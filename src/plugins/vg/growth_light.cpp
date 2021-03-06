/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "growth_light.h"
#include <base/exception.h>
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(GrowthLight)

/*! \class GrowthLight
 * \brief Growth light
 *
 * The characteristics of the lamps are set by the _type_.
 * Inputs
 * ------
 * - _type_ is the type of growth light [HPSL, LED]
 * - _intensity_ is the power of installed lamps per greenhouse area [W/m<SUP>2</SUP>]
 * - _ballastCorrection_ is set to >1 (e.g, 1.15) if capacity includes ballast (>=1)
 * - _age_ is the age of the lamps at simulation start [h]
 * - _lifeTime_ is the age at which light output is reduced to 90% [h]
 * - _on_ tells whether the light should be on [true/false]
 * - _timeStep_ is the integration time step [s]
 *
 * Outputs
 * ------
 * - _currentPeriod_ duration of the current period with light on [min]
 * - _totalPeriod_ total period with light on [h]
 */

GrowthLight::GrowthLight(QString name, QObject *parent)
    : GrowthLightBase(name, parent)
{
    Input(type).equals("HPSL");
    Input(intensity).equals(40.);
    Input(ballastCorrection).equals(1.);
    Input(age).equals(0);
    Input(lifeTime).equals(12000.);
    Input(on).imports("controllers/growthLight[signal]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(currentPeriod);
    Output(totalPeriod);
}

void GrowthLight::reset() {
    // Set parameters according to type
    QString key = type.toLower();
    if (key == "hpsl") {
        attributes.heatCoef = 0.23;
        attributes.longWaveCoef = 0.42;
        attributes.shortWaveCoef = 0.50;
        attributes.parCoef = 0.31;
        attributes.minPeriodOn = 30.;
    }
    else if (key == "led") {
        attributes.heatCoef = 0.02;
        attributes.longWaveCoef = 0.05;
        attributes.shortWaveCoef = 0.82;
        attributes.parCoef = 0.82;
        attributes.minPeriodOn = 0.;
    }
    else
        ThrowException("Unknown growth light type").value(type).context(this);
    // Compute derived parameters
    double netCapacity = intensity/ballastCorrection;
    netAttributes.heatEmission = attributes.heatCoef*netCapacity;
    netAttributes.longWaveEmission = attributes.longWaveCoef*netCapacity;
    netAttributes.shortWaveEmission = attributes.shortWaveCoef*netCapacity;
    netAttributes.parEmission = attributes.parCoef*netCapacity;
    netAttributes.energyFlux = ballastCorrection*intensity;
    degradationRate = log(0.9)/lifeTime;
    currentPeriod = 0.;
    totalPeriod = age;
    noLight();
}

void GrowthLight::update() {
    currentlyOn = on ||
                  ( currentlyOn && (currentPeriod < attributes.minPeriodOn) );
    if (currentlyOn) {
        currentPeriod += timeStep/60.;
        totalPeriod += timeStep/3600.;
        double f = exp(degradationRate*(age+totalPeriod));
        heatEmission = netAttributes.heatEmission + (1-f)*(netAttributes.longWaveEmission+netAttributes.shortWaveEmission);
        longWaveEmission = f*netAttributes.longWaveEmission;
        shortWaveEmission = f*netAttributes.shortWaveEmission;
        totalEmission = heatEmission + longWaveEmission + shortWaveEmission;
        parEmission = f*netAttributes.parEmission;
        energyFlux = netAttributes.energyFlux;
    }
    else
        noLight();
}

void GrowthLight::noLight() {
    heatEmission =
    longWaveEmission =
    shortWaveEmission =
    totalEmission =
    parEmission =
    energyFlux = 0.;
    currentlyOn = false;
}

} //namespace

