#include <base/publish.h>
#include <base/vector_op.h>
#include "honey_supply_growth.h"

using namespace base;
using namespace vector_op;

namespace apis {

PUBLISH(HoneySupplyGrowth)

HoneySupplyGrowth::HoneySupplyGrowth(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes net honey of supply for larval growth");
    Input(cohortDemands).imports("demand/growth[cohortDemands]");
    Input(cost).imports("demand/growth[cost]");
    Output(cohortSupplies).help("Net honey of supply for larval growth over this time step, excluding conversion cost (g)");
}

void HoneySupplyGrowth::update() {
    product(cohortSupplies , cohortDemands, 1-cost);
}

}
