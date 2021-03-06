/*
 * Copyright (C) 2010-2014 O01eg <o01eg@yandex.ru> 
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
#include <list>
#include "individual.h"
#include "world.h"
#include "conf.h"

inline signed long antioverflow_plus(signed long x, signed long y)
{
	const signed long z = x + y;
	const signed long f = (x ^ y) >> (sizeof(signed long) * 8 - 1); // -1 if different sign 
	const signed long f1 = (~(x ^ z)) >> (sizeof(signed long) * 8 - 1); // -1 if same sign
	return (z & (f | f1)) | (x & (~(f | f1)));
}

const size_t MAX_CIRCLES = Config::Instance().GetSLong("max-evalation-loops", 1000); ///< Maximum of eval circles.
const size_t MAX_STEPS = Config::Instance().GetSLong("max-individual-steps", 16); ///< Maximum of any moves.

const char* Individual::Result::ResultNames[Individual::Result::STATUS_VARIABLES] =
{
	"ST_DIRS_MIN_3",
	"ST_AREA_SIZE", ///< Size of path's area.
	"ST_DIRS", ///< Number of used directions.
	"ST_STATIC_WORLD_ACCESS", ///< Static check for world access.
	"ST_STATIC_MEMORY_ACCESS", ///< Static check for memory access.
	"ST_GOOD_MOVES", ///< Count of good moves.
	"ST_ALL_MOVES", ///< Count of any correct moves.
	"ST_STEPS", ///< Number of steps.
	"ST_MAX_ANSWER_QUALITY", ///< Max of move quality.
	"ST_MIN_ANSWER_QUALITY", ///< Max of move quality.
	"ST_COUNT_MEMORY", ///< Count of different memories.
	"ST_ANSWER_QUALITY", ///< Aggregation of move quality.
	"ST_NEG_ERROR_RESET", ///< Count of memory reset by ERROR.
	"ST_NEG_RESET", ///< Count of memoty reset by not LIST.
	"ST_STATIC_NEG_ERROR_TOTAL", ///< Sum of ERRORs.
	"ST_STATIC_IF_TOTAL", ///< Sum of IFs.
	"ST_NEG_CIRCLES", ///< Sum of least circles.
	"ST_STATIC_FUNC_TOTAL" ///< Count of not-NIL ADFs.
};

std::vector<Individual::Result> Individual::Execute(VM::Environment &env, const std::vector<Individual>& population)
{
	std::vector<Individual::Result> results;
	for(size_t i = 0; i < population.size(); ++ i)
	{
		if(population[i].GetResult().IsTested())
		{
			results.push_back(population[i].GetResult(i));
			continue;
		}
		bool equal_ind = false;
		for(size_t j = 0; j < i; ++ j)
		{
			if(population[j].GetProgram().Save() == population[i].GetProgram().Save())
			{
				equal_ind = true;
				break;
			}
		}
		if(equal_ind)
		{
			Result result(i);
			result.m_Quality[0] = -100;
			results.push_back(result);
			continue;
		}
		World world(env, DATA_DIR "labirint.txt");
		env.SetProgram(population[i].GetProgram());
		VM::Object memory(env);
		size_t circle_count = MAX_CIRCLES;
		Result result(i);
		result.m_Quality[Result::ST_STATIC_IF_TOTAL] = 0;
		result.m_Quality[Result::ST_STATIC_NEG_ERROR_TOTAL] = 0;
		for(size_t f = 0; f < MAX_FUNCTIONS; ++ f)
		{
			result.m_Quality[Result::ST_STATIC_IF_TOTAL] += GP::CountIFs(population[i].GetProgram().GetADF(f));
			result.m_Quality[Result::ST_STATIC_NEG_ERROR_TOTAL] -= GP::CountERRORs(population[i].GetProgram().GetADF(f));
		}
		if(result.m_Quality[Result::ST_STATIC_IF_TOTAL] > 5)
		{
			result.m_Quality[Result::ST_STATIC_IF_TOTAL] = 5;
		}
		result.m_Quality[Result::ST_STATIC_FUNC_TOTAL] = population[i].GetProgram().GetSettedADFs();

		std::stringstream ss;
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
		std::list<VM::Object> inputs;
		std::list<VM::Object> memories;
		memories.push_back(memory);
		size_t step = 0;
		for(; step < MAX_STEPS; ++ step)
		{
			VM::Object input(world.GetCurrentWorld(), memory);
			if(std::find(inputs.begin(), inputs.end(), input) != inputs.end())
			{
				// unuseful move
				break;
			}

			// new input, calculate move
			circle_count = MAX_CIRCLES;
			res = env.Run(input, &circle_count);
			inputs.push_back(std::move(input));
			result.m_Quality[Result::ST_NEG_CIRCLES] = antioverflow_plus(result.m_Quality[Result::ST_NEG_CIRCLES], MAX_CIRCLES - circle_count);
			ss << std::setw(70) << res << std::endl;

			if((! res.IsNIL()) && (res.GetType() == VM::ERROR))
			{
				memory = VM::Object(env);
				result.m_Quality[Result::ST_NEG_ERROR_RESET] --;
				result.m_Quality[Result::ST_NEG_RESET] --;
				if(step == 0)
				{
					result.m_Quality[Result::ST_STATIC_MEMORY_ACCESS] = 0;
					result.m_Quality[Result::ST_STATIC_WORLD_ACCESS] = 0;
					break;
				}
			}
			else if((! res.IsNIL()) && (res.GetType() == VM::LIST))
			{
				memory = res.GetTail();
				if(std::find(memories.begin(), memories.end(), memory) == memories.end())
				{
					memories.push_back(memory);
				}

				VM::Object move(res.GetHead());

				std::map<int, signed long> directions;
				size_t quality = CheckMove(move, directions);

				result.m_Quality[Result::ST_ANSWER_QUALITY] = antioverflow_plus(result.m_Quality[Result::ST_ANSWER_QUALITY], quality);
				result.m_Quality[Result::ST_MAX_ANSWER_QUALITY] = std::max<long int>(result.m_Quality[Result::ST_MAX_ANSWER_QUALITY], quality);
				if(step != 0)
				{
					result.m_Quality[Result::ST_MIN_ANSWER_QUALITY] = std::min<long int>(result.m_Quality[Result::ST_MIN_ANSWER_QUALITY], quality);
				}
				else
				{
					result.m_Quality[Result::ST_MIN_ANSWER_QUALITY] = quality;
				}

				if(! directions.empty())
				{
					auto it = directions.begin();
					int dir = it->first;
					signed long power = it->second;
					bool doubles = false;
					++ it;

					for(; it != directions.end(); ++ it)
					{
						if(power < it->second)
						{
							power = it->second;
							dir = it->first;
							doubles = false;
						}
						else if(power == it->second)
						{
							doubles = true;
						}
					}

					if(! doubles)
					{
						dirs[dir - 1] = true;
						if(world.Move(dir))
						{
							result.m_Quality[Result::ST_GOOD_MOVES] ++;
						}
					}

					result.m_Quality[Result::ST_ALL_MOVES] ++;
				}
			}
			else
			{
				memory = VM::Object(env);
				result.m_Quality[Result::ST_NEG_RESET] --;
			}

		}// end moving

		result.m_Quality[Result::ST_STEPS] = step;

		result.m_Quality[Result::ST_COUNT_MEMORY] = memories.size();

		result.m_Quality[Result::ST_NEG_CIRCLES] *= -1;

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
		result.m_Quality[Result::ST_DIRS_MIN_3] = std::min<long int>(result.m_Quality[Result::ST_DIRS], 3);
		
		result.m_Result = ss.str();
		results.push_back(std::move(result));
	}
	//std::sort(results.begin(), results.end());
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

size_t Individual::CheckMove(const VM::WeakObject &move, std::map<int, signed long>& directions)
{
	directions.clear();
	if(move.IsNIL())
	{
		return 1;
	}
	if(move.GetType() != VM::LIST)
	{
		return 2;
	}
	// move is LIST
	// ( UP RIGHT DOWN LEFT )
	VM::WeakObject l = move;
	size_t quality = 1;
	for(int dir = 1; dir <= 4; ++ dir)
	{
		// head
		VM::WeakObject head = l.GetHead();
		if(head.IsNIL())
		{
			quality += dir;
		}
		else
		{
			if(head.GetType() == VM::INTEGER)
			{
				quality += dir;
				directions.insert({dir, static_cast<signed long>(head.GetValue())});
			}
			else
			{
				directions.clear();
				return quality;
			}
		}
		
		// tail
		VM::WeakObject tail = l.GetTail();
		if(tail.IsNIL())
		{
			return quality;
		}
		else
		{
			if(tail.GetType() == VM::LIST)
			{
				quality += dir;
				l = tail;
			}
			else
			{
				directions.clear();
				return quality;				
			}
		}
	}

	if(l.IsNIL())
	{
		//add positive fitness for perfect directions object.
		quality += 100;
	}
	else
	{
		directions.clear();
	}
	return quality;
}

void Individual::Optimize(const GP::OptimizeRules& rules)
{
	if(! m_Result.IsTested())
	{
		for(int adf_index = MAX_FUNCTIONS; adf_index >= 0; -- adf_index)
		{
			m_Program.SetADF(adf_index, GP::Optimize(m_Program.GetADF(adf_index), m_Program, rules, (adf_index == 0) ? GP::OPT_REQ_LIST : GP::OPT_NONE));
		}
		m_Program.Minimize();
	}
}

