#include <boxes/port.h>
#include <boxes/publish.h>
#include "simulation.h"

using namespace boxes;

namespace base {

PUBLISH(Simulation)

Simulation::Simulation(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(Simulation);
    Input(iterations).equals(1);
    Input(steps).equals(1);
    Output(iteration).noReset();
    Output(step);
}

void Simulation::run() {
    amendFamily();
    initializeFamily();
    for (iteration = 0; iteration < iterations; ++iteration) {
        resetFamily();
        for (step = 0; step < steps; ++step) {
            updateFamily();
        }
        cleanupFamily();
    }
    debriefFamily();
}


}
