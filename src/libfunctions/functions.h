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

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "vm/environment.h"

using namespace VM;

void func_01_cons(const Object& args, Object *result);
void func_02_car(const Object& args, Object *result);
void func_03_cdr(const Object& args, Object *result);
void func_04_is_nil(const Object& args, Object *result);
void func_05_is_int(const Object& args, Object *result);
void func_06_is_func(const Object& args, Object *result);
void func_07_is_adf(const Object& args, Object *result);
void func_08_is_param(const Object& args, Object *result);
void func_09_is_quote(const Object& args, Object *result);
void func_10_is_list(const Object& args, Object *result);
void func_11_int_plus(const Object& args, Object *result);
void func_12_int_minus(const Object& args, Object *result);
void func_13_int_mult(const Object& args, Object *result);
void func_14_int_div(const Object& args, Object *result);
void func_15_int_mod(const Object& args, Object *result);
void func_16_equal(const Object& args, Object *result);
void func_17_and(const Object& args, Object *result);
void func_18_or(const Object& args, Object *result);
void func_19_int_less(const Object& args, Object *result);
void func_20_int_greater(const Object& args, Object *result);
void func_21_int_equal(const Object& args, Object *result);

extern Environment::Func func_array[];

#endif

