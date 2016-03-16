#include <cmath>
#include <base/exception.h>
#include <base/publish.h>
#include "sandy.h"

using namespace std;
using namespace base;

namespace boxes {

PUBLISH(Sandy)

Sandy::Sandy(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(xMin).equals(0);
    Input(xMax).equals(1);
    Input(yMax).equals(1);
    Input(a).equals(1);
    Input(b).equals(1);
    Input(x);
    Output(value).page("");
}

void Sandy::reset() {
    if (xMin >= xMax) {
        QString value{"%1 >= %2"};
        ThrowException("'xMin' must be less than 'xMax'").value(value.arg(xMin).arg(xMax)).context(this);;
    }
    C = (a<=0. || b<=0.) ?  1. : pow(a/(a+b),-a) * pow(b/(a+b),-b);
    update();
}

void Sandy::update() {
    value = (x<=xMin || x>=xMax) ? 0. :
            yMax*C*pow((x-xMin)/(xMax-xMin),a) * pow((xMax-x)/(xMax-xMin),b);
}



}
