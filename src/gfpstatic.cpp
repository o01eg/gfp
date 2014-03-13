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

#include <fstream>
#include <ctime>
#include <set>
#include "vm/ioobject.h"

int main(int, char **)
{
	srand(time(NULL));
	{
		VM::Environment env;
		// Create 3 types: TYPE-A, TYPE-B, TYPE-C
		
		VM::Object type_a = env.DefineSymbol("TYPE-A");
		VM::Object type_b = env.DefineSymbol("TYPE-B");
		VM::Object type_c = env.DefineSymbol("TYPE-C");

		// Typed constants and generated expressions.
		std::unordered_multimap<VM::Object, VM::Object> type_table;
		type_table.insert(
			{{type_a, env.DefineSymbol("A1")}
			,{type_a, env.DefineSymbol("A2")}
			,{type_b, env.DefineSymbol("B1")}
			,{type_b, env.DefineSymbol("B2")}
			,{type_c, env.DefineSymbol("C1")}
			,{type_c, env.DefineSymbol("C2")}});

		// ( A . B ) mean A -> B
		// Add A -> B
		type_table.insert({VM::Object(type_a, type_b), env.DefineSymbol("_AB")});
	}
	return 0;
}
