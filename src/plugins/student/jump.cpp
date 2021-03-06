#include <base/publish.h>
#include "jump.h"

using namespace base;

namespace student {

PUBLISH(Jump)

Jump::Jump(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial).equals(27);
    Output(value);
    Output(finished);
}

void Jump::reset() {
    value = (initial < 1) ? 1 : initial;
    finished = (value == 1);
}

void Jump::update() {
    value = (value%2 == 0) ? value/2 : 3*value + 1;
    finished = (value == 1);
}


}
