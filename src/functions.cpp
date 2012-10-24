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

#include "functions.h"
#include "vm/object.h"

/// \file functions.cpp

FuncData func_array[] =
{
	FuncData(func_01_cons, "CONS", 2, GP::OPT_NONE, GP::OPT_REQ_INT, true),
	FuncData(func_02_car, "CAR", 1, GP::OPT_REQ_LIST, GP::OPT_NONE, false),
	FuncData(func_03_cdr, "CDR", 1, GP::OPT_REQ_LIST, GP::OPT_NONE, false),
	FuncData(func_04_is_nil, "NULL?", 1, GP::OPT_BOOL, GP::OPT_BOOL, false),
	FuncData(func_05_is_int, "INT?", 1, GP::OPT_NONE, GP::OPT_BOOL, false),
	FuncData(func_06_is_func, "FUNC?", 1, GP::OPT_NONE, GP::OPT_BOOL, false),
	FuncData(func_07_is_adf, "ADF?", 1, GP::OPT_NONE, GP::OPT_BOOL, false),
	FuncData(func_08_is_param, "PARAM?", 1, GP::OPT_NONE, GP::OPT_BOOL, false),
	FuncData(func_09_is_quote, "QUOTE?", 1, GP::OPT_NONE, GP::OPT_BOOL, false),
	FuncData(func_10_is_list, "LIST?", 1, GP::OPT_NONE, GP::OPT_BOOL, false),
	FuncData(func_11_int_plus, "+", 2, GP::OPT_REQ_INT, GP::OPT_REQ_LIST, true),
	FuncData(func_12_int_minus, "-", 2, GP::OPT_REQ_INT, GP::OPT_REQ_LIST, true),
	FuncData(func_13_int_mult, "*", 2, GP::OPT_REQ_INT, GP::OPT_REQ_LIST, true),
	FuncData(func_14_int_div, "DIV", 2, GP::OPT_REQ_INT, GP::OPT_REQ_LIST, true),
	FuncData(func_15_int_mod, "MOD", 2, GP::OPT_REQ_INT, GP::OPT_REQ_LIST, true),
	FuncData(func_16_equal, "EQ", 2, GP::OPT_NONE, GP::OPT_BOOL, false),
	FuncData(func_17_and, "AND", 2, GP::OPT_BOOL, GP::OPT_BOOL, false),
	FuncData(func_18_or, "OR", 2, GP::OPT_BOOL, GP::OPT_BOOL, false),
	FuncData(func_19_int_less, "<", 2, GP::OPT_REQ_INT, GP::OPT_BOOL, false),
	FuncData(func_20_int_greater, ">", 2, GP::OPT_REQ_INT, GP::OPT_BOOL, false),
	FuncData(func_21_int_equal, "==", 2, GP::OPT_REQ_INT, GP::OPT_BOOL, false),
	FuncData()
};

/// \brief CONS - Construct list.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_01_cons(const std::vector<Object>& args, Object *result)
{
	(*result) = Object(args[0], args[1]);
}

/// \brief CAR - Get head of list.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_02_car(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if((! arg1.IsNIL()) && (arg1.GetType() == LIST))
	{
		(*result) = arg1.GetHead();
	}
	else
	{
		(*result) = arg1.GetEnv().GetERROR();
	}
}

/// \brief CDR - Get tail of list.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_03_cdr(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if((! arg1.IsNIL()) && (arg1.GetType() == LIST))
	{
		(*result) = arg1.GetTail();
	}
	else
	{
		(*result) = arg1.GetEnv().GetERROR();
	}
}

/// \brief NULL - Predicate: true if arg1 is NIL, can use as NOT.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_04_is_nil(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if(arg1.IsNIL())
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(arg1.GetEnv());
	}
}

/// \brief INT - Predicate: true if arg1 has type Object::INTEGER.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_05_is_int(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if((! arg1.IsNIL()) && (arg1.GetType() == INTEGER))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(arg1.GetEnv());
	}
}

/// \brief FUNC - Predicate: true if arg1 has type Object::FUNC, Object::EVAL or Object::IF.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_06_is_func(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if((! arg1.IsNIL()) && (arg1.GetType() == FUNC) && (arg1.GetType() == EVAL) && (arg1.GetType() == IF))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(arg1.GetEnv());
	}
}

/// \brief ADF - Predicate: true if arg1 has type Object::ADF.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_07_is_adf(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if((! arg1.IsNIL()) && (arg1.GetType() == ADF))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(arg1.GetEnv());
	}
}

/// \brief PARAM - Predicate: true if arg1 has type Object::PARAM.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_08_is_param(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if((! arg1.IsNIL()) && (arg1.GetType() == PARAM))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(arg1.GetEnv());
	}
}

void func_09_is_quote(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if((! arg1.IsNIL()) && (arg1.GetType() == QUOTE))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(arg1.GetEnv());
	}
}

/// \brief LIST - Predicate: true if arg1 has type Object::LIST.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_10_is_list(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	if((! arg1.IsNIL()) && (arg1.GetType() == LIST))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(arg1.GetEnv());
	}
}

/// \brief + - Adding integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_11_int_plus(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == INTEGER) && (arg2.GetType() == INTEGER))
	{
		(*result) = Object(arg1.GetEnv(), INTEGER, static_cast<signed long>(arg1.GetValue()) + static_cast<signed long>(arg2.GetValue()));
	}
	else
	{
		// wrong types
		(*result) = arg1.GetEnv().GetERROR();
	}
}

/// \brief - - Subtract integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_12_int_minus(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == INTEGER) && (arg2.GetType() == INTEGER))
	{
		(*result) = Object(arg1.GetEnv(), INTEGER, static_cast<signed long>(arg1.GetValue()) - static_cast<signed long>(arg2.GetValue()));
	}
	else
	{
		// wrong types
		(*result) = arg1.GetEnv().GetERROR();
	}
}

/// \brief * - Multiply integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_13_int_mult(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == INTEGER) && (arg2.GetType() == INTEGER))
	{
		(*result) = Object(arg1.GetEnv(), INTEGER, static_cast<signed long>(arg1.GetValue()) * static_cast<signed long>(arg2.GetValue()));
	}
	else
	{
		// wrong types
		(*result) = arg1.GetEnv().GetERROR();
	}
}

/// \brief / - Divide integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_14_int_div(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == INTEGER) && (arg2.GetType() == INTEGER) && (arg2.GetValue() != 0))
	{
		(*result) = Object(arg1.GetEnv(), INTEGER, static_cast<signed long>(arg1.GetValue()) / static_cast<signed long>(arg2.GetValue()));
	}
	else
	{
		// wrong types or division by zero
		(*result) = arg1.GetEnv().GetERROR();
	}
}

/// \brief % - Division by mobule on integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_15_int_mod(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == INTEGER) && (arg2.GetType() == INTEGER) && (arg2.GetValue() != 0))
	{
		(*result) = Object(arg1.GetEnv(), INTEGER, static_cast<signed long>(arg1.GetValue()) % static_cast<signed long>(arg2.GetValue()));
	}
	else
	{
		// wrong types or division by zero
		(*result) = arg1.GetEnv().GetERROR();
	}
}

/// \brief EQ - Return not-NIL if types and values are equal.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_16_equal(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if(arg1 == arg2)
	{
		// equal
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
	else
	{
		(*result) = Object(arg1.GetEnv());
	}
}

/// \brief AND - Return not-NIL if both arguments are not-NIL.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_17_and(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if(arg1.IsNIL() || arg2.IsNIL())
	{
		// false
		(*result) = Object(arg1.GetEnv());
	}
	else
	{
		// true
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
}

/// \brief OR - Return not-NIL if less one of arguments is not-NIL.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_18_or(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if(arg1.IsNIL() && arg2.IsNIL())
	{
		// false
		(*result) = Object(arg1.GetEnv());
	}
	else
	{
		// true
		(*result) = Object(arg1.GetEnv(), INTEGER, 0);
	}
}

/// \brief < - Return not-NIL if first argument less than second.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_19_int_less(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == INTEGER) && (arg2.GetType() == INTEGER))
	{
		if(static_cast<signed long>(arg1.GetValue()) < static_cast<signed long>(arg2.GetValue()))
		{
			//true
			(*result) = Object(arg1.GetEnv(), INTEGER, 0);
		}
		else
		{
			//false
			(*result) = Object(arg1.GetEnv());
		}
	}
	else
	{
		// wrong types
		(*result) = arg1.GetEnv().GetERROR();
	}

}

/// \brief > - Return not-NIL if first argument greater than second.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_20_int_greater(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == INTEGER) && (arg2.GetType() == INTEGER))
	{
		if(static_cast<signed long>(arg1.GetValue()) > static_cast<signed long>(arg2.GetValue()))
		{
			//true
			(*result) = Object(arg1.GetEnv(), INTEGER, 0);
		}
		else
		{
			//false
			(*result) = Object(arg1.GetEnv());
		}
	}
	else
	{
		// wrong types
		(*result) = arg1.GetEnv().GetERROR();
	}

}

/// \brief == - Return not-NIL if first argument equal second.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_21_int_equal(const std::vector<Object>& args, Object *result)
{
	const Object& arg1 = args[0];
	const Object& arg2 = args[1];
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == INTEGER) && (arg2.GetType() == INTEGER))
	{
		if(static_cast<signed long>(arg1.GetValue()) == static_cast<signed long>(arg2.GetValue()))
		{
			//true
			(*result) = Object(arg1.GetEnv(), INTEGER, 0);
		}
		else
		{
			//false
			(*result) = Object(arg1.GetEnv());
		}
	}
	else
	{
		// wrong types
		(*result) = arg1.GetEnv().GetERROR();
	}

}


