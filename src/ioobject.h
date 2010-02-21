#ifndef _IO_OBJECT_H_
#define _IO_OBJECT_H_

#include <iostream>
#include "object.h"

std::ostream& operator<<(std::ostream& os, const VM::Object& obj);
std::istream& operator>>(std::istream& is, VM::Object& obj);

#endif

