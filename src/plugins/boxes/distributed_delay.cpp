/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "distributed_delay.h"

using namespace base;

namespace boxes {

DistributedDelay::DistributedDelay(const Parameters &p_, QObject *parent)
    : DistributedDelayBase(parent), p(p_)
//! Create distributed delay from parameters
{
    x.resize(p.k);
}

DistributedDelay::DistributedDelay(const DistributedDelay &dd)
    : DistributedDelayBase(dd.parent), p(dd.p), s(dd.s)
//! Create distributed delay as a copy of existing distributed delay
{
    x = dd.x;
    xSum = dd.xSum;
}

void DistributedDelay::update(double inflow, double dt, double fgr) {
//! Update distributed delay
/*! Add inflow and update by time step (dt). Apply finite growth rate () by 'attrition' mechanism.
*/
    // To compute net change
    double totalBefore = xSum + inflow;

    // Set del and attrition according to Vansickle
    double  del = p.L*pow(fgr, -1./p.k),
            atr = p.k*(1./p.L - 1./del);		// atr <= 0 always

    // Calculate attrition factor
    double b = 1. + atr*del/p.k;

    // Divide time step to increase precision
    int idt = (int) floor(1.5 + 2.*b*dt*p.k/del);
    if (idt < p.minIter) idt = p.minIter;

    // Correct inflow for divided time step
    double dividedInflow = inflow/idt;

    // Calculate flow coefficient
    double a = p.k/del*dt/idt;
    if (!(0.<a && a<=1.)) {
        QString msg = "Illegal value for flow coefficient in DistributedDelay (a==%1). "
                "Should be within ]0;1]. Other parameters: k=%2, del=%3, dt=%4, idt=%5, fgr=%6, L=%7";
        ThrowException(msg.arg(a).arg(p.k).arg(del).arg(dt).arg(idt).arg(fgr).arg(p.L));
    }

    // Integrate
    s.outflowRate = 0;
    for (int j = 0; j < idt; j++){
        // Collect outflow
        s.outflowRate += a*x.at(p.k-1);
        // Step backwards through age classes
        for (int i = p.k-1; i > 0; i--)
            x[i] += a*(x.at(i-1) - b*x.at(i));
        // Finish with first age class; enter inflow into that
        x[0] += dividedInflow - a*b*x.at(0);
    }
    xSum = accum(x);
    s.growthRate = xSum + s.outflowRate - totalBefore;
}

DistributedDelay::State DistributedDelay::state() const {
    return s;
}

} // namespace
