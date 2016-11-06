/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_FRACTIONOFRAINRUNOFF_H
#define PESTTOX_FRACTIONOFRAINRUNOFF_H

#include <base/box.h>

namespace PestTox {

class fractionofrainrunoff : public base::Box
{
public:
    fractionofrainrunoff(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Q, P;

    // Output
    double frr;
};

} //namespace


#endif