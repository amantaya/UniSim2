#include <algorithm>
#include <base/exception.h>
#include <base/path.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "population.h"

using namespace base;

namespace boxes {

PUBLISH(Population)

Population::Population(QString name, QObject *parent)
    : Box(name, parent),
      _cohorts(&cohorts),
      _age(&age)
{
    Input(initial).equals(100);
    Input(ageIncrement);
    Input(bufferSize).equals(100);
    Input(firstCohortGain);
    Input(cohortsGain);
    Input(cohortsLoss);

    Output(lastCohortSpill);
    Output(cohorts);
    Output(age);
}

void Population::reset() {
    _cohorts.resize(bufferSize);
    _age.resize(bufferSize);

    _cohorts.push(initial);
    _age.push(0);

    if (!cohortsGain.isEmpty() && cohortsGain.size() != bufferSize) {
        QString msg{"cohortsGain must have same vector size (%1) as bufferSize (%2)"};
        ThrowException(msg.arg(cohortsGain.size()).arg(bufferSize));
    }
    if (!cohortsLoss.isEmpty() && cohortsLoss.size() != bufferSize) {
        QString msg{"cohortsLoss must have same vector size (%1) as bufferSize (%2)"};
        ThrowException(msg.arg(cohortsLoss.size()).arg(bufferSize));
    }
}

void Population::update() {
    if (!cohortsGain.isEmpty())
        vector_op::plus(cohorts, cohortsGain);
    if (!cohortsLoss.isEmpty())
        vector_op::minus(cohorts, cohortsLoss);
    vector_op::plus(age, ageIncrement);
    lastCohortSpill = _cohorts.at(1);
    _cohorts.push(firstCohortGain);
    _age.push(0);
}



}