/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_DATE_TIME_SIGNAL_H
#define VG_DATE_TIME_SIGNAL_H

#include "base_signal.h"

namespace vg {

class DateTimeSignal : public BaseSignal
{
public:
    DateTimeSignal(QString name, QObject *parent);
    double computeSignal();
private:
    // Inputs
    int beginDay, endDay, day;
    QTime beginTime, endTime, time;
    double signalInside, signalOutsideTimeOnly, signalOutside;
    bool circadian;
};

} //namespace
#endif
