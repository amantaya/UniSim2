//#include <boost/circular_buffer/base.hpp>
#include <algorithm>
#include <functional>
#include <base/publish.h>
#include "buffer.h"

using namespace std;
using namespace base;

namespace boxes {

PUBLISH(Buffer)

Buffer::Buffer(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial);
    Input(input);
    Input(increment);
    Input(size).equals(100);
    Output(sum);
    Output(average);
    Output(buffer);
}

void Buffer::reset() {
    sum = average = 0;
    if (size < 1) size = 1;
    buffer.fill(0, size);
    _buffer.set_capacity(size);
    _firstUpdate = true;
}

void Buffer::update() {
    double totalInput = input;
    if (_firstUpdate) {
        totalInput += initial;
        _firstUpdate = false;
    }
    double take = _buffer.full() ? _buffer.front() : 0;
    _buffer.push_back(totalInput);
    if (increment != 0)
        transform(_buffer.begin(), _buffer.end(), _buffer.begin(),
                  [this](double value) { return value + this->increment; });
    int n = _buffer.size();
    sum += totalInput + n*increment- take;
    average = sum/n;
    updateOutputBuffer();
}

void Buffer::updateOutputBuffer() {
    auto range1 = _buffer.array_one(),
         range2 = _buffer.array_two();
    int n1 = range1.second,
        n2 = range2.second;
    const double *source1 = range1.first,
                 *source2 = range2.first;
    double *dest1 = buffer.data();
    copy(source1, source1 + n1, dest1);
    copy(source2, source2 + n2, dest1 + n1);
}

}