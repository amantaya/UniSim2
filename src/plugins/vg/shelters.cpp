/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <base/path.h>
#include <base/publish.h>
#include "shelters.h"
#include "surface_radiation.h"

using std::max;
using namespace base;

namespace vg {

PUBLISH(Shelters)

/*! \class Shelters
 * \brief Collection of all greenhouse shelters
 *
 * Inputs
 * ------
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _heatCapacityCoversPerGround_ is the total heat capacity of all covers [J/kg/m<SUP>2</SUP> ground]
 * - _heatCapacityScreensPerGround_ is the total heat capacity of all screens [J/kg/m<SUP>2</SUP> ground]
 * - _screensEffectiveArea_ is the total drawn of screens [m<SUP>2</SUP>]
 * - _screensPerGroundArea_ is the _screensEffectiveArea_ per _ground area [m<SUP>2</SUP> screen/m<SUP>2</SUP> ground]
 * - _screensMaxState_ is maximum state of all screens [0;1]
 */

Shelters::Shelters(QString name, QObject *parent)
    : ShelterBase(name, parent)
{
    Input(groundArea).imports("geometry[groundArea]");
    Output(heatCapacityCoversPerGround);
    Output(heatCapacityScreensPerGround);
    Output(screensEffectiveArea);
    Output(screensPerGroundArea);
    Output(screensMaxState);
}

#define Pull(p) si.p = shelter->port(#p)->valuePtr<double>()

void Shelters::initialize() {
    infos.clear();
    QVector<ShelterBase*> shelters = Path("./*", this).resolveMany<ShelterBase>();
    for (ShelterBase* shelter : shelters) {
        ShelterInfo si;
        si.sr = shelter->surfaceRadiation();
        Pull(diffuseLightTransmitted);
        Pull(directLightTransmitted);
        Pull(totalLightTransmitted);
        Pull(lightAbsorbedCover);
        Pull(lightAbsorbedScreens);
        Pull(haze);
        Pull(U);
        Pull(airTransmissivity);
        Pull(screensMaxState);
        Pull(area);
        Pull(relativeArea);
        Box *cover = Path("./cover", shelter).resolveOne<Box>(this),
            *screens = Path("./screens", shelter).resolveOne<Box>(this);
        si.heatCapacityCover = cover->port("heatCapacity")->valuePtr<double>();
        si.heatCapacityScreens = screens->port("heatCapacity")->valuePtr<double>();
        si.screensEffectiveArea = screens->port("effectiveArea")->valuePtr<double>();
        infos << si;
    }
}

#define Accumulate(p) p += (*info.p);
#define AccumulateWeighted(p) p += (*info.p) * (*info.relativeArea);
#define AccumulateSr(p) sr.p += info.sr->p * (*info.relativeArea)

void Shelters::update() {
    SurfaceRadiation sr;
    sr.setToZero();
    diffuseLightTransmitted =
    directLightTransmitted =
    totalLightTransmitted =
    lightAbsorbedCover =
    lightAbsorbedScreens =
    U =
    airTransmissivity =
    screensEffectiveArea =
    screensMaxState = 0;
    double heatCapacityCover{0},
           heatCapacityScreens{0};
    for (ShelterInfo info : infos) {
        AccumulateSr(light.inner.abs);
        AccumulateSr(light.inner.ref);
        AccumulateSr(light.outer.abs);
        AccumulateSr(light.outer.ref);
        AccumulateSr(light.tra);
        AccumulateSr(directLight.inner.abs);
        AccumulateSr(directLight.inner.ref);
        AccumulateSr(directLight.outer.abs);
        AccumulateSr(directLight.outer.ref);
        AccumulateSr(directLight.tra);
        AccumulateSr(lw.inner.abs);
        AccumulateSr(lw.inner.ref);
        AccumulateSr(lw.outer.abs);
        AccumulateSr(lw.outer.ref);
        AccumulateSr(lw.tra);

        Accumulate(diffuseLightTransmitted);
        Accumulate(directLightTransmitted);
        Accumulate(totalLightTransmitted);
        Accumulate(lightAbsorbedCover);
        Accumulate(lightAbsorbedScreens);
        AccumulateWeighted(haze);
        AccumulateWeighted(U);
        AccumulateWeighted(airTransmissivity);
        Accumulate(heatCapacityCover);
        Accumulate(heatCapacityScreens);
        Accumulate(screensEffectiveArea);
        if (*info.screensMaxState > screensMaxState)
            screensMaxState = (*info.screensMaxState);
    }
    heatCapacityCoversPerGround = heatCapacityCover/groundArea;
    heatCapacityScreensPerGround = heatCapacityScreens/groundArea;
    screensPerGroundArea = screensEffectiveArea/groundArea;
    set(sr);
}

} //namespace

