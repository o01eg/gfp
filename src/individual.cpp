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
#include "individual.h"
#include "ioobject.h"
#include "ga_utils.h"
#include "world.h"

const size_t MAX_FUNCTIONS = 16;
const size_t MAX_STOPS = 100; ///< Maximum of stop moves

Individual::Individual(const VM::Program &prog)
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
	env.LoadFunctions(DATA_DIR "functions.txt");
	for(size_t i = 0; i < population.size(); i ++)
	{
		World world(env, DATA_DIR "labirint.txt");
		VM::Program prog = population[i].GetProgram(env);
		env.SetProgram(prog);
		VM::Object memory(env);
		bool active = true;
		Result result(i);
		std::stringstream ss;
		size_t max_stops = MAX_STOPS;

		// moving in labirint
		while(active && max_stops)
		{
			size_t circle_count = 1000;
			bool changes = false;
			VM::Object res = env.Run(VM::Object(world.GetCurrentWorld(), memory), &circle_count);
			result.m_Quality[Result::ST_NEG_CIRCLES] += circle_count;
			ss << res << std::endl;
			if(! res.IsNIL())
			{
				switch(res.GetType())
				{
				case VM::LIST:
					{
						VM::WeakObject new_mem = res.GetTail();
						if(new_mem != memory)
						{
							changes = true;
							memory = new_mem;
						}
						VM::WeakObject move = res.GetHead();
						if((! move.IsNIL()) && (move.GetType() == VM::LIST))
						{
							if((! move.IsNIL()) && (move.GetHead().GetType() == VM::INTEGER) && (move.GetHead().GetValue() == 0))
							{
								if((! move.GetTail().IsNIL()) && (move.GetTail().GetType() == VM::LIST))
								{
									VM::WeakObject dir_obj = move.GetTail().GetHead();
									if((! dir_obj.IsNIL()) && (dir_obj.GetType() == VM::INTEGER))
									{
										size_t dir = dir_obj.GetValue();
										if((dir > 0) && (dir <= 4))
										{
											if(world.Move(dir))
											{
												changes = true;
												max_stops = MAX_STOPS;
												std::cout << "Success move: " << res << std::endl;
											}
										}
									}
								}
							}
						}
						break;
					}
				case VM::ERROR:
				default:
					break;
				}
			}

			if(! changes)
			{
				if(memory.IsNIL())
				{
					active = false;
				}
				else
				{
					memory = VM::Object(env);
				}
			}
			max_stops --;
		}

		result.m_Result = ss.str();
		results.push_back(result);
	}
	std::sort(results.begin(), results.end());
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

VM::Program Individual::GetProgram(VM::Environment &env) const
{
	std::stringstream ss(m_ProgramText);
	VM::Object obj(env);
	ss >> obj;
	return VM::Program(obj);
}

