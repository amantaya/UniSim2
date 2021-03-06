/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_EGGSLAID_H
#define PESTTOX_EGGSLAID_H

#include <base/box.h>

namespace PestTox {

class eggslaid : public base::Box
{
public:
    eggslaid(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double eggsPerFemale, sexRatio, population;

    // Output
    double totaleggs;

};

} //namespace


#endif
