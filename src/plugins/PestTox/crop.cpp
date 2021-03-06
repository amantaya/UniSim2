/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop.h"
#include <base/any_year.h>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH(Crop)
	
Crop::Crop(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(sowingDate).equals("1/5/*").help("Date of sowing");
    Input(date).imports("calendar[date]");
    Output(sown).help("Seeds sown today ? (0 or 1)");
}

void Crop::update() {
    sown = equals(sowingDate, date) ? 1 : 0;
}

} //namespace

