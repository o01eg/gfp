#include "../object.h"

/// \file functions.cpp

#ifdef _USRDLL
# define DLLEXPORT __declspec(dllexport)
#else
# define DLLEXPORT 
#endif

using namespace VM;

extern "C"
{
	void DLLEXPORT func_00_eval(const Object& args, Object *result);
	void DLLEXPORT func_01_cons(const Object& args, Object *result);
	void DLLEXPORT func_02_car(const Object& args, Object *result);
	void DLLEXPORT func_03_cdr(const Object& args, Object *result);
	void DLLEXPORT func_04_is_nil(const Object& args, Object *result);
	void DLLEXPORT func_05_is_int(const Object& args, Object *result);
	void DLLEXPORT func_06_is_func(const Object& args, Object *result);
	void DLLEXPORT func_07_is_adf(const Object& args, Object *result);
	void DLLEXPORT func_08_is_param(const Object& args, Object *result);
	void DLLEXPORT func_09_is_quote(const Object& args, Object *result);
	void DLLEXPORT func_10_is_if(const Object& args, Object *result);
	void DLLEXPORT func_11_is_list(const Object& args, Object *result);
	void DLLEXPORT func_12_int_plus(const Object& args, Object *result);
	void DLLEXPORT func_13_int_minus(const Object& args, Object *result);
	void DLLEXPORT func_14_int_mult(const Object& args, Object *result);
	void DLLEXPORT func_15_int_div(const Object& args, Object *result);
	void DLLEXPORT func_16_int_mod(const Object& args, Object *result);
	void DLLEXPORT func_17_equal(const Object& args, Object *result);
	void DLLEXPORT func_18_and(const Object& args, Object *result);
	void DLLEXPORT func_19_or(const Object& args, Object *result);
	void DLLEXPORT func_20_int_less(const Object& args, Object *result);
	void DLLEXPORT func_21_int_greater(const Object& args, Object *result);
	void DLLEXPORT func_22_int_equal(const Object& args, Object *result);
}

/// \brief EVAL - Calculate value of expression in arg1.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_00_eval(const Object& args, Object *result)
{
	(*result) = args.GetEnv().Eval(args.GetHead());
}

/// \brief CONS - Construct list.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_01_cons(const Object& args, Object *result)
{
	(*result) = Object(args.GetHead(), args.GetTail().GetHead());
}

/// \brief CAR - Get head of list.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_02_car(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::LIST))
	{
		(*result) = arg1.GetHead();
	}
	else
	{
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}
}

/// \brief CDR - Get tail of list.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_03_cdr(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::LIST))
	{
		(*result) = arg1.GetTail();
	}
	else
	{
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}
}

/// \brief NULL - Predicate: true if arg1 is NIL, can use as NOT.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_04_is_nil(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if(arg1.IsNIL())
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(args.GetEnv());
	}
}

/// \brief INT - Predicate: true if arg1 has type Object::INTEGER.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_05_is_int(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::INTEGER))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(args.GetEnv());
	}
}

/// \brief FUNC - Predicate: true if arg1 has type Object::FUNC.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_06_is_func(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::FUNC))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(args.GetEnv());
	}
}

/// \brief ADF - Predicate: true if arg1 has type Object::ADF.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_07_is_adf(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::ADF))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(args.GetEnv());
	}
}

/// \brief PARAM - Predicate: true if arg1 has type Object::PARAM.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_08_is_param(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::PARAM))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(args.GetEnv());
	}
}

/// \brief QUOTE - Predicate: true if arg1 has type Object::QUOTE.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_09_is_quote(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::QUOTE))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(args.GetEnv());
	}
}

/// \brief IF - Predicate: true if arg1 has type Object::IF.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_10_is_if(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::IF))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(args.GetEnv());
	}
}

/// \brief LIST - Predicate: true if arg1 has type Object::LIST.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_11_is_list(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	if((! arg1.IsNIL()) && (arg1.GetType() == Object::LIST))
	{
		// return true
		// ! 0 is true because it isn't NIL
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		// return false
		(*result) = Object(args.GetEnv());
	}
}

/// \brief + - Adding integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_12_int_plus(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == Object::INTEGER) && (arg2.GetType() == Object::INTEGER))
	{
		(*result) = Object(args.GetEnv(), Object::INTEGER, arg1.GetValue() + arg2.GetValue());
	}
	else
	{
		// wrong types
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}
}

/// \brief - - Subtract integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_13_int_minus(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == Object::INTEGER) && (arg2.GetType() == Object::INTEGER))
	{
		(*result) = Object(args.GetEnv(), Object::INTEGER, arg1.GetValue() - arg2.GetValue());
	}
	else
	{
		// wrong types
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}
}

/// \brief * - Multiply integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_14_int_mult(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == Object::INTEGER) && (arg2.GetType() == Object::INTEGER))
	{
		(*result) = Object(args.GetEnv(), Object::INTEGER, arg1.GetValue() * arg2.GetValue());
	}
	else
	{
		// wrong types
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}
}

/// \brief / - Divide integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_15_int_div(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == Object::INTEGER) && (arg2.GetType() == Object::INTEGER) && (arg2.GetValue() != 0))
	{
		(*result) = Object(args.GetEnv(), Object::INTEGER, arg1.GetValue() / arg2.GetValue());
	}
	else
	{
		// wrong types or division by zero
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}
}

/// \brief % - Division by mobule on integers.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_16_int_mod(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == Object::INTEGER) && (arg2.GetType() == Object::INTEGER) && (arg2.GetValue() != 0))
	{
		(*result) = Object(args.GetEnv(), Object::INTEGER, arg1.GetValue() % arg2.GetValue());
	}
	else
	{
		// wrong types or division by zero
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}
}

/// \brief EQ - Return not-NIL if types and values are equal.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_17_equal(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if(arg1 == arg2)
	{
		// equal
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
	else
	{
		(*result) = Object(args.GetEnv());
	}
}

/// \brief AND - Return not-NIL if both arguments are not-NIL.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_18_and(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if(arg1.IsNIL() || arg2.IsNIL())
	{
		// false
		(*result) = Object(args.GetEnv());
	}
	else
	{
		// true
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
}

/// \brief OR - Return not-NIL if less one of arguments is not-NIL.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_19_or(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if(arg1.IsNIL() && arg2.IsNIL())
	{
		// false
		(*result) = Object(args.GetEnv());
	}
	else
	{
		// true
		(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
	}
}

/// \brief < - Return not-NIL if first argument less than second.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_20_int_less(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == Object::INTEGER) && (arg2.GetType() == Object::INTEGER))
	{
		if(arg1.GetValue() < arg2.GetValue())
		{
			//true
			(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
		}
		else
		{
			//false
			(*result) = Object(args.GetEnv());
		}
	}
	else
	{
		// wrong types
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}

}

/// \brief > - Return not-NIL if first argument greater than second.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_21_int_greater(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == Object::INTEGER) && (arg2.GetType() == Object::INTEGER))
	{
		if(arg1.GetValue() > arg2.GetValue())
		{
			//true
			(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
		}
		else
		{
			//false
			(*result) = Object(args.GetEnv());
		}
	}
	else
	{
		// wrong types
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}

}

/// \brief == - Return not-NIL if first argument equal second.
/// \param args List of arguments.
/// \param result Pointer to result.
void func_22_int_equal(const Object& args, Object *result)
{
	const Object& arg1 = args.GetHead();
	const Object& arg2 = args.GetTail().GetHead();
	if((! arg1.IsNIL()) && (! arg2.IsNIL()) && (arg1.GetType() == Object::INTEGER) && (arg2.GetType() == Object::INTEGER))
	{
		if(arg1.GetValue() == arg2.GetValue())
		{
			//true
			(*result) = Object(args.GetEnv(), Object::INTEGER, 0);
		}
		else
		{
			//false
			(*result) = Object(args.GetEnv());
		}
	}
	else
	{
		// wrong types
		(*result) = Object(args.GetEnv(), Object::ERROR);
	}

}


