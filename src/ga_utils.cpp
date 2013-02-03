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

#include "ga_utils.h"
#include "vm/program.h"
#include "conf.h"
//#include "vm/ioobject.h"

const size_t MAX_DEPTH = Config::Instance().GetSLong("max-object-depth", 8);
const size_t MAX_OPT_LOOPS = Config::Instance().GetSLong("max-opt-loops", 512);
const size_t MUTATION_RATE_PERCENT = Config::Instance().GetSLong("mutation-rate-percent", 10);

size_t GP::CountIFs(const VM::WeakObject& obj)
{
	std::stack<VM::WeakObject> stack;
	size_t result = 0;
	stack.push(obj);
	while(! stack.empty())
	{
		VM::WeakObject t = stack.top();
		stack.pop();

		if(! t.IsNIL())
		{
			switch(t.GetType())
			{
			case VM::LIST:
				if(! t.GetHead().IsNIL())
				{
					if(t.GetHead().GetType() == VM::QUOTE)
					{
						break; //switch(t.GetType()): doesn't count IFs in ( QUOTE ... )
					}
				}
				while((! t.IsNIL()) && (t.GetType() == VM::LIST))
				{
					stack.push(t.GetHead());

					t = t.GetTail();
				}
				break;
			case VM::IF:
				++ result;
				break;
			default:
				break;
			}
		}
	}
	return result;
}

size_t GP::CountERRORs(const VM::WeakObject& obj)
{
	std::stack<VM::WeakObject> stack;
	size_t result = 0;
	stack.push(obj);
	while(! stack.empty())
	{
		VM::WeakObject t = stack.top();
		stack.pop();

		if(! t.IsNIL())
		{
			switch(t.GetType())
			{
			case VM::LIST:
				while((! t.IsNIL()) && (t.GetType() == VM::LIST))
				{
					stack.push(t.GetHead());

					t = t.GetTail();
				}
				break;
			case VM::ERROR:
				++ result;
				break;
			default:
				break;
			}
		}
	}
	return result;
}

bool GP::IsContainParam(const VM::WeakObject &obj)
{
	std::stack<VM::WeakObject> stack;
	bool result = false;
	stack.push(obj);
	while(! stack.empty())
	{
		VM::WeakObject t = stack.top();
		stack.pop();

		if(! t.IsNIL())
		{
			switch(t.GetType())
			{
			case VM::LIST: // get list
				if(! t.GetHead().IsNIL())
				{
					if(t.GetHead().GetType() == VM::QUOTE)
					{
						break; //switch(t.GetType()): doesn't store ( QUOTE ... )
					}
					if(t.GetHead().GetType() == VM::IF)
					{
						if((! t.GetTail().IsNIL()) && (t.GetTail().GetType() == VM::LIST))
						{
							stack.push(t.GetTail().GetHead());
						}
						break; //switch(t.GetType()): check only condition
					}
				}
				// fill stack
				while((! t.IsNIL()) && (t.GetType() == VM::LIST))
				{
					stack.push(t.GetHead());

					t = t.GetTail();
				}
				break;
			case VM::PARAM:
				return true;
			default:
				break; //do nothing
			}
		}
	}
	return result;
}

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
					case VM::ERROR:
					case VM::INTEGER:
					case VM::FUNC:
					case VM::ADF:
					case VM::PARAM:
					case VM::LIST:
					case VM::EVAL:
					case VM::SYMBOL:
					case VM::MACRO:
						break; // do nothing, only avoid warning
					}
				}
				while((! obj.IsNIL()) && (obj.GetType() == VM::LIST))
				{
					stack.push(obj.GetHead());
					obj = obj.GetTail();
				}
				stack.push(obj); // here obj is not LIST
			}
			else
			{
				if(obj.GetType() == VM::PARAM)
				{
					return true;
				}
			}
		}
	}
	return result;
}

VM::Object GP::GenerateObj(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, size_t depth, size_t current_adf)
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
		res = env.GetPARAM();
		break;
	case 4: // quote
		res = env.GetQUOTE();
		break;
	case 5: // list
		res = VM::Object(GP::GenerateObj(env, funcs, depth + 1, current_adf), GP::GenerateObj(env, funcs, depth + 1, current_adf));
		break;
	}
	return res;
}

VM::Object GP::GenerateExec(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, size_t depth, size_t current_adf)
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
		res = env.GetPARAM();
		break;
	case 3: // ( ' object )
		res = VM::Object(GP::GenerateObj(env, funcs, depth + 1, current_adf), std::move(res));
		res = VM::Object(env.GetQUOTE(), std::move(res));
		break;
	case 4: // ( %ADF arg )
		res = VM::Object(GP::GenerateExec(env, funcs, depth + 1, current_adf), std::move(res)); // ( arg )
		res = VM::Object(VM::Object(env, VM::ADF, current_adf + rand() % (MAX_FUNCTIONS - current_adf + 1)), std::move(res));
		break;
	default: // call
		{
			size_t func_num = rand() % funcs.size();
			for(size_t param = 0; param < funcs[func_num].second; ++ param)
			{
				res = VM::Object(GenerateExec(env, funcs, depth + 1, current_adf), std::move(res));
			}
			res = VM::Object(funcs[func_num].first, std::move(res));
		}
		break;
	}
	return res;
}

VM::Object GP::Optimize(const VM::Object& obj, VM::Program& prog, const OptimizeRules& rules, const OptimizeMode mode)
{
	VM::Environment &env = obj.GetEnv();
	VM::Object res(env);
	env.SetProgram(prog);

	if(GP::IsContainParam(obj))
	{
		// something containg PARAM not in QUOTE
		if(obj.GetType() == VM::PARAM)
		{
			return obj;
		}
		else
		{
			// obj is LIST not ( QUOTE ... )
			// form is ( ADF|FUNC|IF|EVAL args... )
			if((! obj.IsNIL()) && (obj.GetType() == VM::LIST))
			{
				VM::Object head = obj.GetHead();
				// ( IF check no-check no-check ) | ( ADF|FUNC|EVAL check...)
				const bool check_no_if = head.IsNIL() || (head.GetType() != VM::IF);
				bool check_1st = true;
				VM::Object t = obj.GetTail();
				std::stack<VM::Object> stack;

				/// \todo Rewrite to foreach.
				while((! t.IsNIL()) && (t.GetType() == VM::LIST))
				{
					VM::Object opt(env);
					if(check_no_if)
					{
						std::unordered_map<VM::Object, OptimizeMode>::const_iterator it = rules.returnERRORat.find(head);
						if((it != rules.returnERRORat.end()) && (mode != OPT_BOOL) && (mode & it->second))
						{
							//std::cerr << "Error mode=" << mode << " head=" << head << std::endl;
							return env.GetERROR();
						}
						if((mode == OPT_BOOL) && rules.return0atBOOL.count(head))
						{
							return VM::Object(env, VM::INTEGER, 0);
						}

						// get mode for arguments
						OptimizeMode mode_arg = OPT_NONE;
						it = rules.mode.find(head);
						if(it != rules.mode.end())
						{
							mode_arg = it->second;
						}

						opt = GP::Optimize(t.GetHead(), prog, rules, mode_arg);
					}
					else // get IF
					{
						if(check_1st)
						{
							opt = GP::Optimize(t.GetHead(), prog, rules, OPT_BOOL);
						}
						else
						{
							// other args must support current mode.
							opt = GP::Optimize(t.GetHead(), prog, rules, mode);
						}
					}

					if(check_no_if || check_1st)
					{
						check_1st = false;
						if((! opt.IsNIL()) && (opt.GetType() == VM::ERROR))
						{
							//std::cerr << "Error mode=" << mode << " opt=" << opt << std::endl;
							return opt;
						}
					}
					stack.push(std::move(opt));

					t = t.GetTail();
				} //while((! t.IsNIL()) && (t.GetType() == VM::LIST))

				t = VM::Object(env);
				while(! stack.empty())
				{
					t = VM::Object(std::move(stack.top()), std::move(t));
					stack.pop();
				}
				if(! GP::IsContainParam(t))
				{
					return GP::Optimize(VM::Object(std::move(head), std::move(t)), prog, rules, mode);
				}

				if(! check_no_if) // get IF, t.GetTail() = ( TRUE-BRANCH FALSE-BRANCH )
				{
					if(t.GetTail().GetHead() == t.GetTail().GetTail().GetHead()) // if TRUE-BRACH equal to FALSE-BRANCH
					{
						return t.GetTail().GetHead();
					}
				}

				return VM::Object(std::move(head), std::move(t));
			}
		}
	}
	else //if(GP::IsContainParam(obj))
	{
		size_t circle_count = MAX_OPT_LOOPS;
		res = env.Eval(obj, &circle_count);
		if((! res.IsNIL()) && (res.GetType() == VM::ERROR))
		{
			//std::cerr << "Error mode=" << mode << " obj=" << obj << std::endl;
			return res; // return ERROR for wrong code.
		}
		else // get value.
		{
			if(res.IsNIL())
			{
				// not allow at OPT_REQ_INT and OPT_REQ_LIST
				if((mode == GP::OPT_REQ_INT) || (mode == GP::OPT_REQ_LIST))
				{
					//std::cerr << "Error mode=" << mode << " obj=" << obj << " res=NIL" << std::endl;
					return env.GetERROR();
				}
				return res;
			}
			else // no-NIL
			{
				if(mode == GP::OPT_BOOL)
				{
					return VM::Object(env, VM::INTEGER, 0);
				}
				if((mode == GP::OPT_REQ_LIST) && (res.GetType() != VM::LIST))
				{
					//std::cerr << "Error mode=" << mode << " obj=" << obj << " res=" << res << std::endl;
					return env.GetERROR();
				}
				if((mode == GP::OPT_REQ_INT) && (res.GetType() != VM::INTEGER))
				{
					//std::cerr << "Error mode=" << mode << " obj=" << obj << " res=" << res << std::endl;
					return env.GetERROR();
				}

				if(res.GetType() == VM::INTEGER)
				{
					return res;
				}
				else
				{
					return VM::Object(env.GetQUOTE(), VM::Object(std::move(res), VM::Object(env)));
				}
			}
		}
	}// if(GP::IsContainParam(obj))

	// Something wrong
	return env.GetERROR();
}

VM::Object GP::Mutation(const VM::Object& obj, bool is_exec, const std::vector<std::pair<VM::Object, size_t> > &funcs, size_t depth, size_t current_adf)
{
	VM::Object res(obj.GetEnv());
	bool err = (! obj.IsNIL()) && (obj.GetType() == VM::ERROR);
	if(is_exec)
	{
		if(err || ((rand() % 100) <= MUTATION_RATE_PERCENT))
		{
			res = GP::GenerateExec(obj.GetEnv(), funcs, depth, current_adf);
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
					res = GP::Mutation(obj.GetTail().GetHead(), false, funcs, depth + 1, current_adf);
					res = VM::Object(std::move(res), VM::Object(obj.GetEnv()));
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
						res = VM::Object(GP::Mutation(stack.top(), true, funcs, depth + 1, current_adf), std::move(res));
						stack.pop();
					}
				}
				res = VM::Object(std::move(head), std::move(res));
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
		if(err || ((rand() % 100) <= MUTATION_RATE_PERCENT))
		{
			res = GP::GenerateObj(obj.GetEnv(), funcs, depth, current_adf);
		}
		else
		{
			// no change or go deeper
			if((! obj.IsNIL()) && (obj.GetType() == VM::LIST))
			{
				res = VM::Object(GP::Mutation(obj.GetHead(), false, funcs, depth + 1, current_adf), GP::Mutation(obj.GetTail(), false, funcs, depth + 1, current_adf));
			}
			else
			{
				res = obj;
			}
		}
	}
	return res;
}

VM::Program GP::GenerateProg(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> >& funcs, const OptimizeRules& rules)
{
	VM::Program res(env);
	for(int adf_index = MAX_FUNCTIONS; adf_index >= 0; -- adf_index)
	{
		VM::Object adf(env);
		do
		{
			adf = GP::GenerateExec(env, funcs, 0, adf_index);
		}
		while(! GP::CheckForParam(adf));
		res.SetADF(adf_index, adf);
		res.SetADF(adf_index, GP::Optimize(adf, res, rules, (adf_index == 0) ? GP::OPT_REQ_LIST : GP::OPT_NONE));
	}
	res.Minimize();
	return res;
}

VM::Program GP::MutateProg(const VM::Program &prog, const std::vector<std::pair<VM::Object, size_t> >& funcs, const OptimizeRules& rules)
{
	VM::Environment& env = prog.GetEnv();
	VM::Program res(env);
	for(int adf_index = MAX_FUNCTIONS; adf_index >= 0; -- adf_index)
	{
		VM::Object adf = prog.GetADF(adf_index);
		if(adf.IsNIL())
		{
			do
			{
				adf = GP::GenerateExec(env, funcs, 0, adf_index);
			}
			while(! GP::CheckForParam(adf));
		}
		else
		{
			VM::Object new_adf(env);
			do
			{
				new_adf = GP::Mutation(adf, true, funcs, 0, adf_index);
			}
			while((! GP::CheckForParam(new_adf)));
			adf = new_adf;
		}
		res.SetADF(adf_index, adf);
		res.SetADF(adf_index, GP::Optimize(adf, res, rules, (adf_index == 0) ? GP::OPT_REQ_LIST : GP::OPT_NONE));
	}
	res.Minimize();
	return res;
}

VM::Program GP::CrossoverProg(const VM::Program &prog1, const VM::Program &prog2, const OptimizeRules& rules)
{
	VM::Environment &env = prog1.GetEnv();
	if(&env != &prog2.GetEnv())
	{
		THROW("Different environments in crossovered programs");
	}
	VM::Program res(env);
	for(int adf_index = MAX_FUNCTIONS; adf_index >= 0; -- adf_index)
	{
		VM::Object adf(env);
		if(prog1.GetADF(adf_index).IsNIL())
		{
			if(! prog2.GetADF(adf_index).IsNIL())
			{
				adf = prog2.GetADF(adf_index);
			}
			// else if both NIL then result also NIL
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
					adf = prog1.GetADF(adf_index);

				}
				else
				{
					adf = ((rand() % 2) ? prog1 : prog2).GetADF(adf_index);
				}
			}
		}
		res.SetADF(adf_index, adf);
		res.SetADF(adf_index, GP::Optimize(adf, res, rules, (adf_index == 0) ? GP::OPT_REQ_LIST : GP::OPT_NONE));
	}
	res.Minimize();
	return res;
}

