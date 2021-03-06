#ifndef BASE_INITIALIZE_H
#define BASE_INITIALIZE_H
#include "port_transform.h"
#include "port_type.h"


namespace base {


template <class T> void initialize(T *destPtr) { *destPtr = T(); }

void initialize(PortType destT, void *destPtr);



} //namespace

#endif
