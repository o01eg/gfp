/*
 * Copyright (C) 2010-2013 O01eg <o01eg@yandex.ru>
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

#include <algorithm>
#include <fstream>
#include "individual.h"
#include "world.h"
#include "current_state.h"
#include "conf.h"
#include "ga_utils.h"

inline signed long antioverflow_plus(signed long x, signed long y)
{
	const signed long z = x + y;
	const signed long f = (x ^ y) >> (sizeof(signed long) * 8 - 1); // -1 if different sign 
	const signed long f1 = (~(x ^ z)) >> (sizeof(signed long) * 8 - 1); // -1 if same sign
	return (z & (f | f1)) | (x & (~(f | f1)));
}

const size_t MAX_STOPS = Config::Instance().GetSLong("max-stop-moves", 1); ///< Maximum of stop moves.
const size_t MAX_CIRCLES = Config::Instance().GetSLong("max-evalation-loops", 1000); ///< Maximum of eval circles.
const size_t MAX_STEPS = Config::Instance().GetSLong("max-individual-steps", 1); ///< Maximum of any moves.

void Individual::CalculateProg(VM::Environment &env, const VM::Program& prog, Individual::Result &result)
{
	VM::Environment::UseProgram use_curr(env, prog);
	CurrentState::UseProgram use_st(prog);

	World world(env);
	VM::Object memory(env);
	VM::Object prev_res(env);
	VM::Object prev_move(env);
	signed long prev_dir = 0;
	bool active = true;
	size_t circle_count = MAX_CIRCLES;

	result.m_Quality[Result::ST_IF_TOTAL] = 0;
	result.m_Quality[Result::ST_NEG_ERROR_TOTAL] = 0;
	for(size_t f = 0; f < MAX_FUNCTIONS; ++ f)
	{
		result.m_Quality[Result::ST_IF_TOTAL] += GP::CountIFs(prog.GetADF(f));
		result.m_Quality[Result::ST_NEG_ERROR_TOTAL] -= GP::CountERRORs(prog.GetADF(f));
	}
	if(result.m_Quality[Result::ST_IF_TOTAL] > 3)
	{
		result.m_Quality[Result::ST_IF_TOTAL] = 3;
	}
	std::stringstream ss;
	size_t max_stops = MAX_STOPS;
	bool dirs[4] = {false, false, false, false};

	// static check (memory)
	VM::Object res = env.Run(VM::Object(world.GetCurrentWorld(), env.GetERROR()), &circle_count);
	if((! res.IsNIL()) && (res.GetType() == VM::ERROR) && circle_count)
	{
		result.m_Quality[Result::ST_STATIC_MEMORY_ACCESS] = 1;
	}

	circle_count = MAX_CIRCLES;
	res = env.Run(VM::Object(world.GetErrorWorld(env), VM::Object(env)), &circle_count);
	if((! res.IsNIL()) && (res.GetType() == VM::ERROR) && circle_count)
	{
		result.m_Quality[Result::ST_STATIC_WORLD_ACCESS] = 1;
	}

	// moving in labirint
	for(size_t step = 0; active && max_stops && CurrentState::IsRun() && (step < MAX_STEPS); ++ step)
	{
		circle_count = MAX_CIRCLES;
		bool changes = false;
		res = env.Run(VM::Object(world.GetCurrentWorld(), memory), &circle_count);
		if(! prev_res.IsNIL())
		{
			if(res != prev_res)
			{
				result.m_Quality[Result::ST_ANSWER_CHANGES] ++;
			}
		}
				
		if((! res.IsNIL()) && (res.GetType() == VM::ERROR))
		{
			// it's simply bugged program
			result.m_Quality[Result::ST_STATIC_MEMORY_ACCESS] = 0;
			result.m_Quality[Result::ST_STATIC_WORLD_ACCESS] = 0;
			active = false;
		}
			
		prev_res = res;
		if(circle_count >= MAX_CIRCLES - 1000)
		{
			circle_count = MAX_CIRCLES;
		}
		result.m_Quality[Result::ST_NEG_CIRCLES] += circle_count - MAX_CIRCLES;
		ss << std::setw(70) << res << std::endl;
		if(! res.IsNIL())
		{
			switch(res.GetType())
			{
			case VM::LIST:
				{
					VM::Object new_mem = res.GetTail();
					if(new_mem != memory)
					{
						changes = true;
						memory = std::move(new_mem);
					}
					VM::Object move = res.GetHead();
					if(! prev_move.IsNIL())
					{
						if(move != prev_move)
						{
							result.m_Quality[Result::ST_MOVE_CHANGES] ++;
						}
					}
					prev_move = move;
					signed long code = 0, direction = 0;
					size_t quality = CheckMove(move, &code, &direction);

					if(quality >= 6) // got code
					{
						const signed long CODE_VALUE = -15000;
						result.m_Quality[Result::ST_MOVE_DIFF] = antioverflow_plus(result.m_Quality[Result::ST_MOVE_DIFF], (((code - CODE_VALUE) >= 0) ? -1L : 1L) * (code - CODE_VALUE));
						if(quality == 8) // got direction
						{
							const signed long MIN_DIR = 10000;
							const signed long MAX_DIR = MIN_DIR + 3;
							result.m_Quality[Result::ST_DIR_DIFF] = antioverflow_plus(result.m_Quality[Result::ST_DIR_DIFF], (direction < MIN_DIR) ? (direction - MIN_DIR) : ((direction > MAX_DIR) ? (MAX_DIR - direction) : 0));
							if(prev_dir)
							{
								if((direction % 4 + 1) != prev_dir)
								{
									result.m_Quality[Result::ST_DIR_CHANGES] ++;
								}
							}
							prev_dir = direction % 4 + 1;

							if((code == CODE_VALUE) && (direction >= MIN_DIR) && (direction <= MAX_DIR))
							{
								quality = 100;
							}
							if(world.Move(direction % 4 + 1))
							{
								dirs[direction % 4] = true;
								changes = true;
								max_stops = MAX_STOPS;
								result.m_Quality[Result::ST_GOOD_MOVES] ++;
							}
							result.m_Quality[Result::ST_SUM_MOVES] ++;
						}
					}
					result.m_Quality[Result::ST_ANSWER_QUALITY] = antioverflow_plus(result.m_Quality[Result::ST_ANSWER_QUALITY], quality);
					if(quality < 8)
					{
						// Individual must return good moves.
						active = false;
					}
				}
				break;
			case VM::ERROR:
				break;
			default:
				result.m_Quality[Result::ST_ANSWER_QUALITY] += 1;
				break;
			}
		}

		if(! changes)
		{
			active = false;
		}
		else
		{
			result.m_Quality[Result::ST_STATE_CHANGES]++;
		}
		max_stops --;
	}// end moving

	result.m_Quality[Result::ST_AREA_SIZE] = world.GetAreaSize();
	if(result.m_Quality[Result::ST_AREA_SIZE] > 1)
	{
		ss << world;
	}
	for(size_t i = 0; i < 4; ++ i)
	{
		if(dirs[i])
		{
			result.m_Quality[Result::ST_DIRS] ++;
		}
	}

	result.m_Result = ss.str();
}

void Individual::ThreadedExecute(VM::Environment &env, const VM::WeakObject code, Individual::Result &result)
{
	std::stringstream ss;
	ss << code;

	VM::Object prog_obj(env);
	ss >> prog_obj;
	VM::Program prog(prog_obj);

	CalculateProg(env, prog, result);
}

std::vector<Individual::Result> Individual::Execute(ThreadPool &thread_pool, VM::Environment &env, const std::vector<Individual>& population)
{
	std::vector<Individual::Result> results;
	results.reserve(population.size());

	for(size_t i = 0; (i < population.size()) && CurrentState::IsRun(); ++ i)
	{
		const VM::Program& prog = population[i].GetProgram();
		if(population[i].GetResult().IsTested())
		{
			results.push_back(population[i].GetResult(i));
			continue;
		}
		bool equal_ind = false;
		for(size_t j = 0; j < i; ++ j)
		{
			if(population[j].GetProgram().Save() == prog.Save())
			{
				equal_ind = true;
				break;
			}
		}
		if(equal_ind)
		{
			Result result(i);
			result.m_Quality[0] = -100;
			results.push_back(std::move(result));
			continue;
		}

		Result r(i);
		auto it = results.insert(results.end(), std::move(r));

		if(thread_pool.IsEmpty())
		{
			// std::bind doesn't convert Object to weakObject in this thread. Be sure that std::bind in AddTask get WeakObject.
			thread_pool.AddTask(std::bind(Individual::ThreadedExecute, std::placeholders::_1, static_cast<const VM::WeakObject>(prog.Save()), *it));
			continue;
		}
		
		CalculateProg(env, prog, *it);
	} // for(size_t i

	thread_pool.WaitFinish();
#if 0	
	std::cout << "====\n";
	std::cout << results[0].m_Quality[0] << std::endl;
	std::cout << results[0].m_Quality[1] << std::endl;
	std::cout << results[0].m_Quality[2] << std::endl;
	std::cout << results[0].m_Quality[3] << std::endl;
	std::cout << results[0].m_Result << std::endl;
#endif
	return results;
}

size_t Individual::CheckMove(const VM::WeakObject &move, signed long *code, signed long *direction)
{
	if(move.IsNIL())
	{
		return 2;
	}
	if(move.GetType() != VM::LIST)
	{
		return 3;
	}
	// move is LIST
	VM::WeakObject code_obj = move.GetHead();
	if(code_obj.IsNIL())
	{
		return 4;
	}
	if(code_obj.GetType() != VM::INTEGER)
	{
		return 5;
	}
	if(code)
	{
		(*code) = static_cast<signed long>(code_obj.GetValue());
	}
	VM::WeakObject dir_obj = move.GetTail();
	if(dir_obj.IsNIL())
	{
		return 6;
	}
	if(dir_obj.GetType() != VM::INTEGER)
	{
		return 7;
	}
	if(direction)
	{
		(*direction) = static_cast<signed long>(dir_obj.GetValue());
	}
	return 8;
}

