/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef THRESHOLD_SIGNAL_H
#define THRESHOLD_SIGNAL_H

#include "base_signal.h"

namespace vg {

class ThresholdSignal : public BaseSignal
{
public:
    ThresholdSignal(QString name, QObject *parent);
    double computeSignal();
private:
    double threshold, input, signalBelow, signalAbove;
};

} //namespace
#endif
