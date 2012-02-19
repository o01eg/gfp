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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <vm/environment.h>
#include <vm/object.h>
#include <vm/ioobject.h>
#include <vm/logger.h>

struct TypeSystem
{
	TypeSystem(VM::Environment& env)
		:type_int(env.DefineSymbol("TYPE:INT"))
		,type_bool(env.DefineSymbol("TYPE:BOOL"))
		,value_true(env.DefineSymbol("TRUE"))
		,value_false(env.DefineSymbol("FALSE"))
	{
		symbols.insert(env.DefineSymbol("TYPE:SYM-DIV-BY-ZERO"));
		symbols.insert(env.DefineSymbol("TYPE:EMPTY-LIST"));
	}

	std::set<VM::Object, VM::WeakObject::Comparator> symbols;

	VM::Object type_int;
	VM::Object type_bool;
	VM::Object value_true;
	VM::Object value_false;
};

/// @todo Get type for expression
/// @todo Generate expression for type include functional

VM::Object type_to_expr(const TypeSystem& type_system, const VM::Object& type)
{
	VM::Object expr(type.GetEnv(), VM::ERROR);
	switch(type.GetType())
	{
		case VM::SYMBOL:
			{
				// Generate TYPE:INTEGER
				if(type == type_system.type_int)
				{
					signed long value = static_cast<signed long>(rand()) - RAND_MAX / 2;
					expr = VM::Object(type.GetEnv(), VM::INTEGER, static_cast<VM::Heap::UInt>(value));
					return expr;
				}

				// Generate TYPE:BOOL
				if(type == type_system.type_bool)
				{
					return (rand() % 2) ? type_system.value_true : type_system.value_false;
				}
				
				// Generate TYPE:SYM-*
				std::set<VM::Object>::const_iterator it = type_system.symbols.find(type);
				if(it != type_system.symbols.end())
				{
					return *it;
				}
			}
			break;
		case VM::ERROR:
		case VM::INTEGER:
		case VM::FUNC:
		case VM::ADF:
		case VM::PARAM:
		case VM::QUOTE:
		case VM::IF:
		case VM::LIST:
		case VM::EVAL:
		case VM::MACRO:
		default:
			std::cout << "Wrong or unsupported type " << type.GetType() << std::endl;
	}
	return expr;
}

VM::Object expr_to_type(const TypeSystem& type_system, const VM::Object& expr)
{
	VM::Object type(expr.GetEnv(), VM::ERROR);
	switch(expr.GetType())
	{
		case VM::INTEGER:
			return type_system.type_int;
			break;
		case VM::SYMBOL:
			{
				// check for TYPE:BOOL
				if((expr == type_system.value_true) || (expr == type_system.value_false))
				{
					return type_system.type_bool;
				}

				// check for TYPE:SYM-*
				std::set<VM::Object>::const_iterator it = type_system.symbols.find(expr);
				if(it != type_system.symbols.end()) // Get typed symbols
				{
					return *it;
				}
				/// @todo Other symbols.
			}
			break;
		case VM::ERROR:
		case VM::FUNC:
		case VM::ADF:
		case VM::PARAM:
		case VM::QUOTE:
		case VM::IF:
		case VM::LIST:
		case VM::EVAL:
		case VM::MACRO:
		default:
			std::cout << "Wrong or unsupported type " << expr.GetType() << std::endl;
	}
	return type;
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	{
		VM::Environment env;

		TypeSystem type_system(env);

		if((argc >= 2) && (argv[1][0] == 't')) // type to expression
		{
			VM::Object type(env);
			std::cin >> type;
			std::cout << type_to_expr(type_system, type) << std::endl;
		}
		else // expression to type
		{
			VM::Object expr(env);
			std::cin >> expr;
			std::cout << expr_to_type(type_system, expr) << std::endl;
		}
	}
	return 0;
}
