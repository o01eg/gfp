#include "object.h"

/// \file functions.cpp
/// \todo Write all needed functions.
/// \todo Write description for functions.

using namespace VM;

extern "C"
{
	void func_00_eval(const Object& args, Object *result);
	void func_01_cons(const Object& args, Object *result);
	void func_02_car(const Object& args, Object *result);
	void func_03_cdr(const Object& args, Object *result);
	void func_04_is_nil(const Object& args, Object *result);
	void func_12_int_plus(const Object& args, Object *result);
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

/// \brief NULL - Predicate: true if arg1 is NIL.
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

