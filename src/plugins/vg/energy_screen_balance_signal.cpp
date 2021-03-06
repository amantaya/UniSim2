/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_balance_signal.h"
#include <base/publish.h>

using namespace base;

namespace vg {
	
PUBLISH(EnergyScreenBalanceSignal)

/*! \class EnergyScreenBalanceSignal
 * \brief Calculates the energy balance for drawing a screen
 *
 * Inputs
 * ------
 * - _Ucover_ is the average heat transfer coefficient of the greenhouse cover per ground area [W/K/m<SUP>2</SUP>]
 * - _indoorsLight_ is the intensity of indoors light [W/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 */

EnergyScreenBalanceSignal::EnergyScreenBalanceSignal(QString name, QObject *parent)
    : BaseSignal(name, parent)
{
    Input(Ucover).imports("construction/shelters[U]");
    Input(indoorsLight).imports("indoors/light[total]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
}

double EnergyScreenBalanceSignal::computeSignal()  {
    return (indoorsLight > Ucover*(indoorsTemperature - outdoorsTemperature)) ? 0 : 1;
}

} //namespace
