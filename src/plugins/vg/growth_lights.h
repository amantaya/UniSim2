/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GROWTH_LIGHTS_H
#define VG_GROWTH_LIGHTS_H

#include <QVector>
#include "growth_light_base.h"

namespace vg {

class GrowthLights : public GrowthLightBase
{
public:
    GrowthLights(QString name, QObject *parent);
};
} //namespace


#endif
