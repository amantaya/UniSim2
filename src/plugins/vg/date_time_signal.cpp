/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "date_time_signal.h"

using namespace base;

namespace vg {

PUBLISH(DateTimeSignal)

/*! \class DateTimeSignal
 * \brief Sets a signal according to date and time of day
 *
 *
 * Inputs
 * ------
 * - _beginDay_ is the beginning of the date interval [1..365]
 * - _endDay_ is the end of the date interval [1..365]
 * - _beginTime_ is the beginning of the time interval [hh:mm]
 * - _endTime_ is the end of the time interval [hh:mm]
 * - _day_ is the actual date [1..365]
 * - _time_ is the actual time [hh:mm]
 * - _signalInside_ is the resulting _signal_ when _day_ and _time_ are inside the day and time intervals [-]
 * - _signalOutside_ is the resulting _signal_ when _day_ or _time_ is outside their respective intervals [-]
 * - _signalOutsideTimeOnly_ is the resulting _signal_ when _day_ is inside the date interval but _time_ is outside the time interval [-]
 * - _circadian_ tells whether signal follows a daily rythm. See below. [true,false]
 *
 * Outputs
 * ------
 * See BaseSignal
 *
 * The _circadian_ flag tells whether the time interval _beginTime_ to _endTime_ should be interpreted as a circadian (i.e.
 * daily) rythm. This allows the resulting signal to be *periodic* on a daily basis. Alternatively, when _cirdadian_ is false,
 * the signal will be specified by *one* period beginning on beginDay at beginTime and ending on endDay at endTime.
 * In all cases the intervals are closed, i.e. limit values are considered inside the interval.
 *
 * If _beginDay_-_endDay_ are set to 10/1/2012-20/1/2012 and _beginTime_-_endTime_ are set to 8:00-16:00, these are the
 * values that signal will be set to depending on the current date and time:
 * Date and time | signal
 * ------------- | ------
 * 15/01/2012 12:00	| signalInside
 * 15/01/2012 22:00	| circadian ? signalOutsideTimeOnly : signalOutside
 * 10/01/2012 12:00	| signalInside
 * 10/01/2012 06:00	| signalOutside
 * 20/01/2012 12:00	| signalInside
 * 20/01/2012 22:00	| signalOutside
 */

DateTimeSignal::DateTimeSignal(QString name, QObject *parent)
    : BaseSignal(name, parent){
    Input(beginDay).equals(1);
    Input(endDay).equals(365);
    Input(beginTime).equals(QTime(0,0));
    Input(endTime).equals(QTime(24,0));
    Input(day).imports("calendar[dayOfYear]");
    Input(time).imports("calendar[time]");
    Input(signalOutside).equals(0.);
    Input(signalOutsideTimeOnly).equals(0.);
    Input(signalInside).equals(1.);
    Input(circadian).equals(true);
}

inline QDate date(int day) {
    return QDate(2000,12,31).addDays(day);
}

double DateTimeSignal::computeSignal() {
    bool nowOnDay = (beginDay < endDay) ?
                (day >= beginDay) && (day <= endDay) :
                (day >= beginDay) || (day <= endDay);
    bool nowOnTime = (beginTime < endTime) ?
                (time >= beginTime) && (time <= endTime) :
                (time >= beginTime) || (time <= endTime);
    double result;
    if (circadian) {
        result =  nowOnDay ?
                  (nowOnTime ? signalInside : (signalOutsideTimeOnly))
                  : signalOutside;
    }
    else {
        QDateTime begin(date(beginDay), beginTime),
                  end(date(endDay), endTime),
                  now(date(day), time);
        result = (now >= begin && now <= end) ?
                 signalInside : signalOutside;
    }
    return result;
}

} //namespace

