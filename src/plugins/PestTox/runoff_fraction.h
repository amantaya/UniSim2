/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RUNOFF_FRACTION_H
#define RUNOFF_FRACTION_H

#include <base/box.h>

namespace PestTox {

class RunoffFraction : public base::Box
{
public:
    RunoffFraction(QString name, QObject *parent);
    void update();

private:
    // Input
    double Q, P;

    // Output
    double frr;
};

} //namespace


#endif
