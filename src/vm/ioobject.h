/*
 * Copyright (C) 2010-2011 O01eg <o01eg@yandex.ru> 
 *
 *  This file is part of Genetic Function Programming.
 *
 *  Genetic Function Programming is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Genetic Function Programming is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Genetic Function Programming.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _IO_OBJECT_H_
#define _IO_OBJECT_H_

#include <iostream>
#include "object.h"

/// \brief Dump object data
/// \param[in] obj Object.
/// \param[in] width Width of string.
void dump_object(const VM::WeakObject& obj, std::streamsize width);

std::ostream& operator<<(std::ostream& ostr, const VM::WeakObject& obj);

std::istream& operator>>(std::istream& is, VM::Object& obj);

#endif

