/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QDate>
#include <base/box.h>

namespace PestTox {

class Application : public base::Box
{
public:
    Application(QString name, QObject *parent);
    void update();

private:
    // Input
    QDate applicationDate, date;
    double concentration, rate;

    // Output
    double dose;
};

} //namespace


#endif
