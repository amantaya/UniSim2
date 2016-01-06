#include <limits>
#include <stdlib.h>
#include <string>
#include <QDate>
#include <QDateTime>
#include <QStringList>
#include <QTime>
#include <QVector>
#include "assign.h"
#include "convert.h"
#include "exception.h"

using std::max;
using std::min;
using std::numeric_limits;

namespace boxes {

#define DEST_PTR(destT) \
    reinterpret_cast<destT*>(destPtr)

#define DEST(destT) \
    (*(reinterpret_cast<destT*>(destPtr)))

#define SOURCE_PTR(sourceT) \
    reinterpret_cast<const sourceT*>(sourcePtr)

#define SOURCE(sourceT) \
    (*(reinterpret_cast<const sourceT*>(sourcePtr)))

template <class destT, class sourceT>
void assignToScalarFromScalar(void *destPtr, const void *sourcePtr, PortTransform) {
    DEST(destT) = convert<destT>(SOURCE(sourceT));
}

template <class destT, class sourceT>
void assignToScalarFromVector(void *destPtr, const void *sourcePtr, PortTransform transform) {
    const QVector<sourceT> *sourceVector = SOURCE_PTR(QVector<sourceT>);
    const sourceT *sourceData = sourceVector->data();
    int i = 0, n = sourceVector->size();
    sourceT value;
    switch (transform) {
    case Identity:
        if (n != 1)
            throw Exception("Cannot assign to scalar from vector. Expected vector size = 1", QString::number(n) + " =! 1");
        value = *sourceData;
        break;
    case Sum:
        value = 0;
        while (i++<n)
            value += (*sourceData++);
        break;
    case Average:
        value = 0;
        while (i++<n)
            value += (*sourceData++);
        value /= n;
        break;
    case Min:
        value = numeric_limits<sourceT>::max();
        while (i++<n)
            value += min(value, *sourceData++);
        break;
    case Max:
        value = numeric_limits<sourceT>::min();
        while (i++<n)
            value += max(value, *sourceData++);
        break;
    case All:
        value = true;
        while (value && i++<n)
            value = value && (*sourceData++);
        break;
    case Any:
        value = false;
        while (!value && i++<n)
            value = value || (*sourceData++);
        break;
    case Copy:
    case Split:
        throw Exception("Transform cannot be applied when assigning a vector to a scalar", nameOf(transform));
    }
    DEST(destT) = convert<destT>(value);
}

#define CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(destT, sourceT) \
template <> void assignToScalarFromVector<destT, sourceT>(void *, const void *, PortTransform) { \
    throw Exception("Cannot convert " #sourceT "Vector to " #destT); \
}

#define CANNOT_ASSIGN_TO_ANY_SCALAR_FROM_VECTOR(sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(bool, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(char, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(int, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(long int, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(long long int, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(float, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(double, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(long double, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(QString, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(QDate, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(QTime, sourceT) \
CANNOT_ASSIGN_TO_SCALAR_FROM_VECTOR(QDateTime, sourceT)
//
CANNOT_ASSIGN_TO_ANY_SCALAR_FROM_VECTOR(QString)
CANNOT_ASSIGN_TO_ANY_SCALAR_FROM_VECTOR(QDate)
CANNOT_ASSIGN_TO_ANY_SCALAR_FROM_VECTOR(QTime)
CANNOT_ASSIGN_TO_ANY_SCALAR_FROM_VECTOR(QDateTime)

// Bypass division for Split transform below
QString operator/(QString x, int) { return x; }
QDate operator/(QDate x, int) { return x; }
QTime operator/(QTime x, int) { return x; }
QDateTime operator/(QDateTime x, int) { return x; }


template <class destT, class sourceT>
void assignToVectorFromScalar(void *destPtr, const void *sourcePtr, PortTransform transform) {
    if (transform != Identity)
        throw Exception("Expected 'Identity' transform when assigning to vector", nameOf(transform));
    QVector<destT> *destVector = DEST_PTR(QVector<destT>);
    destT *destData = destVector->data();
    int n = destVector->size();
    sourceT source = SOURCE(sourceT);
    if (n == 1)
        *destData = convert<destT>(source);
    else if (transform == Copy) {
        destVector->fill(convert<destT>(source));
    }
    else if (transform == Split) {
        if (n>0) destVector->fill(convert<destT>(source/n));
    }
    else {
        QString msg("When assigning a scalar to a vector, vector size must be one 1,"
                    "\nor scalar must be transformed by Copy or Split"),
                value("Vector size = %1, Transform = %2");
        throw Exception(msg, value.arg(QString::number(n)).arg(nameOf(transform)));
    }
}

#define ASSIGN_TO_VECTOR_FROM_VECTOR_INTRO(destT, sourceT) \
    if (transform != Identity) \
        throw Exception("Expected 'Identity' transform when assigning Vector to Vector", nameOf(transform)); \
\
    QVector<destT> *destVector = DEST_PTR(QVector<destT>); \
    const QVector<sourceT> *sourceVector = SOURCE_PTR(QVector<sourceT>); \
\
    int n = destVector->size(), \
        n2 = sourceVector->size(); \
    if (n != n2) \
        throw Exception("Cannot assign vectors of unequal lengths", QString::number(n) + " =! " + QString::number(n2))

template <class destT, class sourceT>
void assignToVectorFromVector(void *destPtr, const void *sourcePtr, PortTransform transform) {
    ASSIGN_TO_VECTOR_FROM_VECTOR_INTRO(destT, sourceT);
    destT *destData = destVector->data();
    const sourceT *sourceData = sourceVector->data();
    int i = 0;
    while (i++<n)
        *destData++ = convert<destT>(*sourceData++);
}

#define ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(T) \
template <> \
void assignToVectorFromVector<T,T>(void *destPtr, const void *sourcePtr, PortTransform transform) { \
    ASSIGN_TO_VECTOR_FROM_VECTOR_INTRO(T, T); \
    *destVector = *sourceVector; \
}

ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(bool)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(char)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(int)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(long int)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(long long int)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(float)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(double)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(long double)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(QString)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(QDate)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(QTime)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(QDateTime)

#define ASSIGN_TO_SCALAR(destT, sourceT, sourcePortT, destPtr, sourcePtr, transform) \
case sourcePortT: \
    assignToScalarFromScalar<destT,sourceT>(destPtr, sourcePtr, transform); \
    break; \
case sourcePortT##Vector: \
    assignToScalarFromVector<destT,sourceT>(destPtr, sourcePtr, transform); \
    break;

#define ASSIGN_TO_VECTOR(destT, sourceT, sourcePortT, destPtr, sourcePtr, transform) \
case sourcePortT: \
    assignToVectorFromScalar<destT,sourceT>(destPtr, sourcePtr, transform); \
    break; \
case sourcePortT##Vector: \
    assignToVectorFromVector<destT,sourceT>(destPtr, sourcePtr, transform); \
    break;

#define CANNOT_ASSIGN_FROM_NULL \
case Null: \
    throw Exception("Cannot assign from Null"); \
    break;

#define CASE_ASSIGN_TO_SCALAR(destT) \
switch (sourceT) { \
    ASSIGN_TO_SCALAR(destT, bool, Bool, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, char, Char, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, int, Int, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, long int, LongInt, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, long long int, LongLongInt, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, float, Float, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, double, Double, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, long double, LongDouble, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, QString, String, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, QDate, Date, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, QTime, Time, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, QDateTime, DateTime, destPtr, sourcePtr, transform) \
    CANNOT_ASSIGN_FROM_NULL \
}

#define CASE_ASSIGN_TO_VECTOR(destT) \
switch (sourceT) { \
    ASSIGN_TO_VECTOR(destT, bool, Bool, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, char, Char, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, int, Int, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, long int, LongInt, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, long long int, LongLongInt, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, float, Float, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, double, Double, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, long double, LongDouble, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, QString, String, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, QDate, Date, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, QTime, Time, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, QDateTime, DateTime, destPtr, sourcePtr, transform) \
    CANNOT_ASSIGN_FROM_NULL \
}

#define CASE_ASSIGN(destPortType, destCppType) \
case destPortType: \
    CASE_ASSIGN_TO_SCALAR(destCppType) \
    break; \
case destPortType##Vector: \
    CASE_ASSIGN_TO_VECTOR(destCppType) \
    break

void assign(PortType destT, void *destPtr, PortType sourceT, const void *sourcePtr, PortTransform transform) {
    switch (destT) {
        CASE_ASSIGN(Bool, bool);
        CASE_ASSIGN(Char, char);
        CASE_ASSIGN(Int, int);
        CASE_ASSIGN(LongInt, long int);
        CASE_ASSIGN(LongLongInt, long long int);
        CASE_ASSIGN(Float, float);
        CASE_ASSIGN(Double, double);
        CASE_ASSIGN(LongDouble, long double);
        CASE_ASSIGN(String, QString);
        CASE_ASSIGN(Date, QDate);
        CASE_ASSIGN(Time, QTime);
        CASE_ASSIGN(DateTime, QDateTime);
    case Null:
        throw Exception("Cannot assign to Null");
        break;
    }
}

} //namespace
