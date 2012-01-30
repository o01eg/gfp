/*
 * Copyright (C) 2010-2012 O01eg <o01eg@yandex.ru> 
 *
 * This file is part of Genetic Function Programming.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

