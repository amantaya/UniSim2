#ifndef PORT_TRANSFORM_H
#define PORT_TRANSFORM_H
#include <QMap>
#include <QString>
#include "convert.h"
#include "exception.h"

namespace base {

enum PortTransform {
    Identity,
    Sum,
    Average,
    Min,
    Max,
    Copy,
    Split,
    All,
    Any
};

template<class T> T convert(PortTransform )  { throw Exception("Cannot only convert port transform to QString"); }

template<> QString convert(PortTransform transform);
template<> PortTransform convert(QString s);

QStringList portTransformNames();

}

#endif
