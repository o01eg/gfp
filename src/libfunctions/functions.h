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
#include "ga_utils.h"

using namespace VM;

void func_01_cons(const std::vector<Object>& args, Object *result);
void func_02_car(const std::vector<Object>& args, Object *result);
void func_03_cdr(const std::vector<Object>& args, Object *result);
void func_04_is_nil(const std::vector<Object>& args, Object *result);
void func_05_is_int(const std::vector<Object>& args, Object *result);
void func_06_is_func(const std::vector<Object>& args, Object *result);
void func_07_is_adf(const std::vector<Object>& args, Object *result);
void func_08_is_param(const std::vector<Object>& args, Object *result);
void func_09_is_quote(const std::vector<Object>& args, Object *result);
void func_10_is_list(const std::vector<Object>& args, Object *result);
void func_11_int_plus(const std::vector<Object>& args, Object *result);
void func_12_int_minus(const std::vector<Object>& args, Object *result);
void func_13_int_mult(const std::vector<Object>& args, Object *result);
void func_14_int_div(const std::vector<Object>& args, Object *result);
void func_15_int_mod(const std::vector<Object>& args, Object *result);
void func_16_equal(const std::vector<Object>& args, Object *result);
void func_17_and(const std::vector<Object>& args, Object *result);
void func_18_or(const std::vector<Object>& args, Object *result);
void func_19_int_less(const std::vector<Object>& args, Object *result);
void func_20_int_greater(const std::vector<Object>& args, Object *result);
void func_21_int_equal(const std::vector<Object>& args, Object *result);

struct FuncData : public Environment::Func
{
	FuncData(FunctionPtr func_, const std::string& name_, unsigned char number_param_, GP::OptimizeMode mode_, GP::OptimizeMode returnERRORat_, bool return0atBOOL_)
		: Environment::Func(func_, name_, number_param_)
		, mode(mode_)
		, returnERRORat(returnERRORat_)
		, return0atBOOL(return0atBOOL_)
	{
	}
	FuncData()
		: Environment::Func()
	{
	}
	GP::OptimizeMode mode;
	GP::OptimizeMode returnERRORat;
	bool return0atBOOL;
};

extern FuncData func_array[];

#endif

