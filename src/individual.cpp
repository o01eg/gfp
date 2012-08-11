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

#include <algorithm>
#include <fstream>
#include "individual.h"
#include "ga_utils.h"
#include "world.h"
#include "current_state.h"
#include "conf.h"
#include "libfunctions/functions.h"

inline signed long antioverflow_plus(signed long x, signed long y)
{
	const signed long z = x + y;
	const signed long f = (x ^ y) >> (sizeof(signed long) * 8 - 1); // -1 if different sign 
	const signed long f1 = (~(x ^ z)) >> (sizeof(signed long) * 8 - 1); // -1 if same sign
	return (z & (f | f1)) | (x & (~(f | f1)));
}

const size_t MAX_FUNCTIONS = Config::Instance().GetSLong("max-individual-adfs", 1); ///< Maximum size of program.
const size_t MAX_STOPS = Config::Instance().GetSLong("max-stop-moves", 1); ///< Maximum of stop moves.
const size_t MAX_CIRCLES = Config::Instance().GetSLong("max-evalation-loops", 1000); ///< Maximum of eval circles.
const size_t MAX_STEPS = Config::Instance().GetSLong("max-individual-steps", 1); ///< Maximum of any moves.

Individual Individual::GenerateRand(VM::Environment &env)
{
	VM::Program prog = GP::GenerateProg(env, MAX_FUNCTIONS);
	//std::clog << "Individual random program created." << std::endl;
	return Individual(prog, {});
}

Individual Individual::Mutation(VM::Environment &env, const Individual& ind)
{
	std::stringstream ss(ind.m_ProgramText);
	VM::Object obj(env);
	ss >> obj;
	VM::Program prog(obj);
	return Individual(GP::MutateProg(prog, MAX_FUNCTIONS), ind.m_Parents);
}

Individual Individual::Crossover(VM::Environment &env, const Individual& ind1, const Individual& ind2)
{
	std::stringstream ss1(ind1.m_ProgramText);
	VM::Object obj(env);
	ss1 >> obj;
	VM::Program prog1(obj);
	std::stringstream ss2(ind2.m_ProgramText);
	ss2 >> obj;
	VM::Program prog2(obj);	
	return Individual(GP::CrossoverProg(prog1, prog2, MAX_FUNCTIONS), {ind1, ind2});
}

std::vector<Individual::Result> Individual::Execute(const std::vector<Individual>& population)
{
	std::vector<Individual::Result> results;
	VM::Environment env;
	env.LoadFunctionsFromArray(func_array);
	for(size_t i = 0; (i < population.size()) && CurrentState::IsRun(); ++ i)
	{
		if(population[i].GetResult().IsTested())
		{
			results.push_back(population[i].GetResult(i));
			continue;
		}
		bool equal_ind = false;
		for(size_t j = 0; j < i; ++ j)
		{
			if(population[j].GetText() == population[i].GetText())
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
		VM::Program prog = population[i].GetProgram(env);
		env.SetProgram(prog);
		CurrentState::s_Program = population[i].GetText();
		VM::Object memory(env);
		VM::Object prev_res(env);
		VM::Object prev_move(env);
		signed long prev_dir = 0;
		bool active = true;
		size_t circle_count = MAX_CIRCLES;
		Result result(i);
		result.m_Quality[Result::ST_IF_TOTAL] = 0;
		for(size_t f = 0; f < MAX_FUNCTIONS; ++ f)
		{
			result.m_Quality[Result::ST_IF_TOTAL] += GP::CountIFs(prog.GetADF(f));
		}
		std::stringstream ss;
		size_t max_stops = MAX_STOPS;
		bool first_move = true;
		bool dirs[4] = {false, false, false, false};

			// moving in labirint
			for(size_t step = 0; active && max_stops && CurrentState::IsRun() && (step < MAX_STEPS); ++ step)
			{
				circle_count = MAX_CIRCLES;
				bool changes = false;
				VM::Object res = env.Run(VM::Object(world.GetCurrentWorld(), memory), &circle_count);
				if(! prev_res.IsNIL())
				{
					if(res != prev_res)
					{
						result.m_Quality[Result::ST_ANSWER_CHANGES] ++;
					}
				}
				if(first_move)
				{
					if((! res.IsNIL()) && (res.GetType() == VM::ERROR))
					{
						// it's simply bugged program
						active = false;
					}
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
								memory = new_mem;
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
					//if(memory.IsNIL())
					//{
						active = false;
					//}
					//else
					//{
					//	memory = VM::Object(env);
					//}
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
		results.push_back(result);
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

VM::Program Individual::GetProgram(VM::Environment &env) const
{
	std::stringstream ss(m_ProgramText);
	VM::Object obj(env);
	ss >> obj;
	return VM::Program(obj);
}

bool Individual::Load(const char* filename, Individual *ind)
{
	std::ifstream f(filename);
	if(f.fail())
	{
		return false;
	}
	VM::Environment env;
	env.LoadFunctionsFromArray(func_array);
	VM::Object obj(env);
	f >> obj;
	if(ind)
	{
		(*ind) = Individual(VM::Program(obj), {});
	}
	return true;
}

