/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_photo_degradation.h"
#include <cmath>
#include <base/data_grid.h>
#include <base/environment.h>
#include <base/interpolate.h>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (LeafPhotoDegradation)
	
LeafPhotoDegradation::LeafPhotoDegradation(QString name, QObject *parent)
    : LossRate(name, parent), _freeRadicalsTable(0)
{
    Input(dayOfYear).imports("calendar[dayOfYear]");
    Input(latitude).imports("calendar[latitude]");
    Input(fileName).help("File with table of (Julian day,latitude)-indexed free radicals concentration");
    Input(kOH).help("Degradation rate (cm3/molecules/h)");
    Output(OHconcentration).help("Free OH radical concentration (molecules/cm3)");
}

void LeafPhotoDegradation::initialize() {
    delete _freeRadicalsTable;
    QString filePath = environment().inputFileNamePath(fileName);
    _freeRadicalsTable = new DataGrid(filePath, this);

}

double LeafPhotoDegradation::computeInstantaneous() {
    OHconcentration = interpolate(*_freeRadicalsTable, latitude, dayOfYear)*1e5; // molecules/cm3
    return kOH*OHconcentration/3600.; // s-1 = cm3/molecules/h * molecules/cm3 / (s/h)
}

} //namespace

