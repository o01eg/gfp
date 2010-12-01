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
#include <cstdlib>
#include "ga_utils.h"
#include "program.h"

const size_t MAX_DEPTH = 32;

bool GP::CheckForParam(const VM::WeakObject &func)
{
	std::stack<VM::WeakObject> stack;
	bool result = false;
	if(func.IsNIL())
	{
		return false;
	}
	if(func.GetType() != VM::LIST)
	{
		return false;
	}
	if(func.GetHead().IsNIL() || func.GetTail().IsNIL())
	{
		return false;
	}
	//check for bad "( FUNC $ )" pattern
	VM::WeakObject func_tail = func.GetTail();
	if((! func_tail.GetHead().IsNIL()) && (func_tail.GetHead().GetType() == VM::PARAM) && func_tail.GetTail().IsNIL())
	{
		return false;
	}
	stack.push(func);
	while(! stack.empty())
	{
		VM::WeakObject obj = stack.top();
		stack.pop();

		if(! obj.IsNIL())
		{
			if(obj.GetType() == VM::LIST)
			{
				VM::WeakObject head = obj.GetHead();
				if(! head.IsNIL())
				{
					switch(head.GetType())
					{
					case VM::QUOTE: // skip all
						continue;
					case VM::IF: // check only condition
						stack.push(obj.GetTail().GetHead());
						continue;
					}
				}
				while((! obj.IsNIL()) && (obj.GetType() == VM::LIST))
				{
					stack.push(obj.GetHead());
					obj = obj.GetTail();
				}
				stack.push(obj); // here obj is not LIST
			}
			if(obj.GetType() == VM::PARAM)
			{
				return true;
			}
		}
	}
	return result;
}

VM::Object GP::GenerateObj(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth)
{
	size_t choose = rand() % ((depth >= MAX_DEPTH) ? 5 : 6); // don't choose LIST on more depth.
	VM::Object res(env);
	switch(choose)
	{
	case 0: // integer
		res = VM::Object(env, VM::INTEGER, static_cast<size_t>(rand() % 0x10001 - 0x8000));
		break;
	case 1: // callable objects
		res = funcs[rand() % funcs.size()].first;
		break;
	case 2: // NIL
		break;
	case 3: // parameter
		res = VM::Object(env, VM::PARAM);
		break;
	case 4: // quote
		res = VM::Object(env, VM::QUOTE);
		break;
	case 5: // list
		res = VM::Object(GP::GenerateObj(env, funcs, depth + 1), GP::GenerateObj(env, funcs, depth + 1));
		break;
	}
	return res;
}

VM::Object GP::GenerateExec(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth)
{
	size_t choose = rand() % ((depth >= MAX_DEPTH) ? 3 : 8); // don't choose LIST on more depth.
	VM::Object res(env);
	switch(choose)
	{
	case 0: // integer
		res = VM::Object(env, VM::INTEGER, static_cast<size_t>(rand() % 0x10001 - 0x8000));
		break;
	case 1: // nil
		break;
	case 2: // parameter
		res = VM::Object(env, VM::PARAM);
		break;
	case 3: // ( ' object )
		res = VM::Object(GP::GenerateObj(env, funcs, depth + 1), res);
		res = VM::Object(VM::Object(env, VM::QUOTE), res);
		break;
	default: // call
		{
			size_t func_num = rand() % funcs.size();
			for(size_t param = 0; param < funcs[func_num].second; param ++)
			{
				res = VM::Object(GenerateExec(env, funcs, depth + 1), res);
			}
			res = VM::Object(funcs[func_num].first, res);
		}
		break;
	}
	return res;
}

VM::Object GP::Mutation(const VM::Object& obj, bool is_exec, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth)
{
	VM::Object res(obj.GetEnv());
	if(is_exec)
	{
		if((rand() % 100) > 80)
		{
			res = GP::GenerateExec(obj.GetEnv(), funcs, depth);
		}
		else
		{
			// no change or go deeper
			if((! obj.IsNIL()) && (obj.GetType() == VM::LIST))
			{
				// mutate arguments of function
				VM::Object head(obj.GetHead());
				if(head.IsNIL())
				{
					THROW("Head of callable list cann't be NIL");
				}
				if(head.GetType() == VM::QUOTE)
				{
					// it's quote, make argument as non executable
					res = GP::Mutation(obj.GetTail().GetHead(), false, funcs, depth + 1);
					res = VM::Object(res, VM::Object(obj.GetEnv()));
				}
				else
				{
					std::stack<VM::Object> stack;
					VM::Object temp(obj.GetTail());
					while((! temp.IsNIL()) && (temp.GetType() == VM::LIST))
					{
						stack.push(temp.GetHead());
						temp = temp.GetTail();
					}
					while(! stack.empty())
					{
						res = VM::Object(GP::Mutation(stack.top(), true, funcs, depth + 1), res);
						stack.pop();
					}
				}
				res = VM::Object(head, res);
			}
			else
			{
				res = obj;
			}
		}
	}
	else
	{
		// non exec mutation
		if((rand() % 100) > 80)
		{
			res = GP::GenerateObj(obj.GetEnv(), funcs, depth);
		}
		else
		{
			// no change or go deeper
			if((! obj.IsNIL()) && (obj.GetType() == VM::LIST))
			{
				res = VM::Object(GP::Mutation(obj.GetHead(), false, funcs, depth + 1), GP::Mutation(obj.GetTail(), false, funcs, depth + 1));
			}
			else
			{
				res = obj;
			}
		}
	}
	return res;
}

VM::Program GP::GenerateProg(VM::Environment &env, size_t max_funcs)
{
	VM::Program res(env);
	std::vector<std::pair<VM::Object, size_t> > funcs;
	funcs.push_back(std::make_pair(VM::Object(env, VM::IF), 3));
	funcs.push_back(std::make_pair(VM::Object(env, VM::EVAL), 1));
	for(size_t i = 0 ; i < env.functions.size(); i ++)
	{
		funcs.push_back(std::make_pair(VM::Object(env, VM::FUNC, i), env.functions[i].number_param));
	}
	for(int adf_index = max_funcs; adf_index >= 0; adf_index --)
	{
		VM::Object adf(env);
		funcs.push_back(std::make_pair(VM::Object(env, VM::ADF, adf_index), 1));
		do
		{
			adf = GP::GenerateExec(env, funcs, 0);
		}
		while(! GP::CheckForParam(adf));
		res.SetADF(adf_index, adf);
	}
	res.Minimize();
	return res;
}

VM::Program GP::MutateProg(const VM::Program &prog, size_t max_funcs)
{
	VM::Environment& env = prog.GetEnv();
	VM::Program res(env);
	std::vector<std::pair<VM::Object, size_t> > funcs;
	funcs.push_back(std::make_pair(VM::Object(env, VM::IF), 3));
	funcs.push_back(std::make_pair(VM::Object(env, VM::EVAL), 1));
	for(size_t i = 0 ; i < env.functions.size(); i ++)
	{
		funcs.push_back(std::make_pair(VM::Object(env, VM::FUNC, i), env.functions[i].number_param));
	}
	for(int adf_index = max_funcs; adf_index >= 0; adf_index --)
	{
		VM::Object adf = prog.GetADF(adf_index);
		funcs.push_back(std::make_pair(VM::Object(env, VM::ADF, adf_index), 1));
		if(adf.IsNIL())
		{
			do
			{
				adf = GP::GenerateExec(env, funcs, 0);
			}
			while(! GP::CheckForParam(adf));
			res.SetADF(adf_index, adf);
		}
		else
		{
			VM::Object new_adf(env);
			do
			{
				new_adf = GP::Mutation(adf, true, funcs, 0);
			}
			while(! GP::CheckForParam(new_adf));
			adf = new_adf;
		}
		res.SetADF(adf_index, adf);
	}
	res.Minimize();
	return res;
}

VM::Program GP::CrossoverProg(const VM::Program &prog1, const VM::Program &prog2, size_t max_funcs)
{
	VM::Environment &env = prog1.GetEnv();
	if(&env != &prog2.GetEnv())
	{
		THROW("Different environments in crossovered programs");
	}
	VM::Program res(env);
	std::vector<std::pair<VM::Object, size_t> > funcs;
	funcs.push_back(std::make_pair(VM::Object(env, VM::IF), 3));
	funcs.push_back(std::make_pair(VM::Object(env, VM::EVAL), 1));
	for(size_t i = 0 ; i < env.functions.size(); i ++)
	{
		funcs.push_back(std::make_pair(VM::Object(env, VM::FUNC, i), env.functions[i].number_param));
	}
	for(int adf_index = max_funcs; adf_index >= 0; adf_index --)
	{
		VM::Object adf(env);
		funcs.push_back(std::make_pair(VM::Object(env, VM::ADF, adf_index), 1));
		if(prog1.GetADF(adf_index).IsNIL())
		{
			if(prog2.GetADF(adf_index).IsNIL())
			{
				// both NIL
				do
				{
					adf = GP::GenerateExec(env, funcs, 0);
				}
				while(! GP::CheckForParam(adf));
			}
			adf = prog2.GetADF(adf_index);
		}
		else
		{
			if(prog2.GetADF(adf_index).IsNIL())
			{
				adf = prog1.GetADF(adf_index);
			}
			else
			{
				// both exist
				if(prog1.GetADF(adf_index) == prog2.GetADF(adf_index))
				{
					//if equal then mutate them
					do
					{
						adf = GP::Mutation(prog1.GetADF(adf_index), true, funcs, 0);
					}
					while(! GP::CheckForParam(adf));

				}
				else
				{
					adf = ((rand() % 2) ? prog1 : prog2).GetADF(adf_index);
				}
			}
		}
		res.SetADF(adf_index, adf);
	}
	res.Minimize();
	return res;
}

