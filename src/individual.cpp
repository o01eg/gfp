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

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include "individual.h"
#include "ioobject.h"
#include "ga_utils.h"
#include "world.h"
#include "current_state.h"
#if COMPILE_STATIC
#include "libfunctions/functions.h"
#endif

const size_t MAX_FUNCTIONS = 32; ///< Maximum size of program.
const size_t MAX_STOPS = 4; ///< Maximum of stop moves.
const size_t MAX_CIRCLES = 10000; ///< Maximum of eval circles.
const size_t MAX_STEPS = 16; ///< Maximum of any moves.

Individual::Individual(const VM::Program &prog)
	:m_Result(-1) // new, not yet tested
{
	std::stringstream ss;
	ss << prog.Save();
	m_ProgramText = ss.str();
}

Individual Individual::GenerateRand(VM::Environment &env)
{
	VM::Program prog = GP::GenerateProg(env, MAX_FUNCTIONS);
	//std::clog << "Individual random program created." << std::endl;
	return Individual(prog);
}

Individual Individual::Mutation(VM::Environment &env, const Individual& ind)
{
	std::stringstream ss(ind.m_ProgramText);
	VM::Object obj(env);
	ss >> obj;
	VM::Program prog(obj);
	return Individual(GP::MutateProg(prog, MAX_FUNCTIONS));
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
	return Individual(GP::CrossoverProg(prog1, prog2, MAX_FUNCTIONS));
}

std::vector<Individual::Result> Individual::Execute(const std::vector<Individual> &population)
{
	std::vector<Individual::Result> results;
	VM::Environment env;
#if COMPILE_STATIC
	env.LoadFunctionsFromArray(func_array);
#else
	env.LoadFunctionsFromFile(DATA_DIR "functions.txt");
#endif
	for(size_t i = 0; (i < population.size()) && CurrentState::IsRun(); i ++)
	{
		if(population[i].GetResult().IsTested())
		{
			results.push_back(population[i].GetResult(i));
			continue;
		}
		bool equal_ind = false;
		for(size_t j = 0; j < i; j ++)
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
		size_t prev_dir = 0;
		bool active = true;
		size_t circle_count = MAX_CIRCLES;
		Result result(i);
		std::stringstream ss;
		size_t max_stops = MAX_STOPS;
		bool first_move = true;

		try
		{
			// moving in labirint
			for(size_t step = 0; active && max_stops && CurrentState::IsRun() && (step < MAX_STEPS); step ++)
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
				result.m_Quality[Result::ST_NEG_CIRCLES] += circle_count;
				ss << std::setw(35) << res << std::endl;
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
							result.m_Quality[Result::ST_ANSWER_QUALITY] = CheckMove(move, &code, &direction);
							if(result.m_Quality[Result::ST_ANSWER_QUALITY] >= 6) // got code
							{
								const signed long CODE_VALUE = -15000;
								result.m_Quality[Result::ST_MOVE_DIFF] = -abs(code - CODE_VALUE);
								if(result.m_Quality[Result::ST_ANSWER_QUALITY] == 8) // got direction
								{
									const signed long MIN_DIR = 10000;
									const signed long MAX_DIR = MIN_DIR + 3;
									result.m_Quality[Result::ST_DIR_DIFF] = (direction < MIN_DIR) ? (direction - MIN_DIR) : ((direction > MAX_DIR) ? (MAX_DIR - direction) : 0);
									if(prev_dir)
									{
										if((direction % 4 + 1) != prev_dir)
										{
											result.m_Quality[Result::ST_DIR_CHANGES] ++;
										}
									}
									prev_dir = direction % 4 + 1;
	
									if((code == CODE_VALUE) && (direction >= MIN_DIR) && (direction <= MAX_DIR) && (result.m_Quality[Result::ST_ANSWER_QUALITY] >= 100))
									{
										result.m_Quality[Result::ST_ANSWER_QUALITY] = 100;
										result.m_Quality[Result::ST_DIR_DIFF] = 0;
									}
									if(world.Move(direction % 4 + 1))
									{
										changes = true;
										max_stops = MAX_STOPS;
										result.m_Quality[Result::ST_GOOD_MOVES] ++;
									}
									result.m_Quality[Result::ST_SUM_MOVES] ++;
								}
							}
						}
						break;
					case VM::ERROR:
						break;
					default:
						result.m_Quality[Result::ST_ANSWER_QUALITY] = 1;
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
			}
		}
		catch(...)
		{
			result.m_Quality[0] = -10000;
			std::cerr << "exception = " << population[i].GetText() << std::endl;
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

Individual Individual::Load(const char* filename)
{
	std::ifstream f(filename);
	if(f.fail())
	{
		throw Glib::Error(1, 0, Glib::ustring::compose("Cann't open file %1", filename));
	}
	VM::Environment env;
#if COMPILE_STATIC
	env.LoadFunctionsFromArray(func_array);
#else
	env.LoadFunctionsFromFile(DATA_DIR "functions.txt");
#endif
	VM::Object obj(env);
	f >> obj;
	return Individual(VM::Program(obj));
}

