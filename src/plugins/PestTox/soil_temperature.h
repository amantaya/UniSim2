/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SOIL_TEMPERATURE_H
#define SOIL_TEMPERATURE_H

#include <base/box.h>

namespace PestTox {

class SoilTemperature : public base::Box
{
public:
    SoilTemperature(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Tmin, Tmax, dayLength;

    // Output
    double value;
};

} //namespace


#endif
