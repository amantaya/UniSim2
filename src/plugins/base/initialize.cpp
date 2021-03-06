#include "exception.h"
#include "initialize.h"

#define DEST(destT) \
    (*(reinterpret_cast<destT*>(destPtr)))

#define DEST_VECTOR(destT) \
    (*(reinterpret_cast<QVector<destT>*>(destPtr)))

namespace {
    template <class T> T zero() {
        T v{};
        return v;
    }

    template <> QDate zero() { return QDate(2001,1,1); }
    template <> QTime zero() { return QTime(0,0,0); }
    template <> QDateTime zero() { return QDateTime(QDate(2001,1,1)); }

    template <class T> void zero(QVector<T> &vector) {
        vector.fill(zero<T>());
    }
}

#define CASE_INIT(PortT, CppT) \
    case PortT: \
        DEST(CppT) = zero<CppT>(); \
        break; \
    case PortT##Vector: \
        zero(DEST_VECTOR(CppT)); \
        break;

namespace base {

void initialize(PortType destT, void *destPtr) {
    switch(destT) {
    CASE_INIT(Bool, bool);
    CASE_INIT(Char, char);
    CASE_INIT(Int, int);
    CASE_INIT(LongInt, long int);
    CASE_INIT(LongLongInt, long long int);
    CASE_INIT(Float, float);
    CASE_INIT(Double, double);
    CASE_INIT(LongDouble, long double);
    CASE_INIT(String, QString);
    CASE_INIT(Date, QDate);
    CASE_INIT(Time, QTime);
    CASE_INIT(DateTime, QDateTime);
    case Null:
        ThrowException("Cannot initialize Null");
        break;
    }
}



} //namespace

