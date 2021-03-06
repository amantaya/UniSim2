/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VAPOUR_FLUX_BASE_H
#define VG_VAPOUR_FLUX_BASE_H

#include <base/box.h>

namespace vg {

class VapourFluxBase : public base::Box
{
public:
    VapourFluxBase(QString name, QObject *parent);
protected:
    // Outputs
    double conductance, gain, vapourFlux;
};
} //namespace


#endif
