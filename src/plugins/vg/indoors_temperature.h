/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_INDOORS_TEMPERATURE_H
#define VG_INDOORS_TEMPERATURE_H

#include <base/box.h>

namespace vg {

class IndoorsTemperature : public base::Box
{
public:
    IndoorsTemperature(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double height, timeStep, energyFlux, resetValue, baseTemperature;
    // Output
    double value;
    // Data
    int tick;
};
} //namespace


#endif
