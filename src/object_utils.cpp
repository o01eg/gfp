/*
 * Copyright (C) 2010 O01eg <o01eg@yandex.ru> 
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

#include <stack>
#include "object_utils.h"

VM::Object EditList(const VM::WeakObject& list, size_t pos, VM::WeakObject& obj)
{
	std::stack<VM::WeakObject> obj_stack;
	VM::WeakObject p(list);
	while((pos > 0) && (! p.IsNIL()))
	{
		obj_stack.push(p.GetHead());
		p = p.GetTail();
		pos --;
	}
	VM::Object res(obj, p.GetTail());
	while(! obj_stack.empty())
	{
		res = VM::Object(obj_stack.top(), res);
		obj_stack.pop();
	}
	return res;
}
