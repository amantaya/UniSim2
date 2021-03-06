/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_LAI_H
#define VG_CROP_LAI_H

#include <base/box.h>

namespace vg {

class CropLai : public base::Box
{
public:
    CropLai(QString name, QObject *parent);
    void reset();
protected:
    // Inputs
    double laiStartPerPlant, fractionPlantArea;
    // Outputs
    double value;
};
} //namespace


#endif
