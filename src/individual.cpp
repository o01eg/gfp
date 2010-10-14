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

const size_t MAX_FUNCTIONS = 4;

Individual::Individual(const VM::Program &prog)
{
	std::stringstream ss;
	ss << prog.Save();
	m_ProgramText = ss.str();
}

Individual Individual::GenerateRand(VM::Environment &env)
{
	VM::Program prog = GP::GenerateProg(env, MAX_FUNCTIONS);
	std::clog << "Individual random program created." << std::endl;
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
//	World world(env, DATA_DIR "labirint.txt");
	for(size_t i = 0; i < population.size(); i ++)
	{
		VM::Program prog = population[i].GetProgram(env);
		env.SetProgram(prog);
		size_t circle_count = 1000;
		VM::Object res = env.Run(VM::Object(env), &circle_count);
		Result result(i);
		std::stringstream ss;
		ss << res;
		result.m_Result = ss.str();
		result.m_Quality[Result::ST_NEG_CIRCLES] = circle_count;
		if(res.IsNIL())
		{
			result.m_Quality[Result::ST_ANSW_NO_ERROR] = 1;
			result.m_Quality[Result::ST_ANSW_IS_INT] = 0;
			result.m_Quality[Result::ST_NEG_ABS_NUM] = 0;
		}
		else
		{
			if(res.GetType() == VM::ERROR)
			{
				result.m_Quality[Result::ST_ANSW_NO_ERROR] = 0;
				result.m_Quality[Result::ST_ANSW_IS_INT] = 0;
				result.m_Quality[Result::ST_NEG_ABS_NUM] = 0;
			}
			else
			{
				if(res.GetType() == VM::INTEGER)
				{
					//std::cout << "res is integer " << res << std::endl;
					int val = static_cast<int>(res.GetValue());
					result.m_Quality[Result::ST_ANSW_NO_ERROR] = 1;
					result.m_Quality[Result::ST_ANSW_IS_INT] = 1;
					result.m_Quality[Result::ST_NEG_ABS_NUM] = (val > 0) ? (-val) : val;
				}
				else
				{
					result.m_Quality[Result::ST_ANSW_NO_ERROR] = 1;
					result.m_Quality[Result::ST_ANSW_IS_INT] = 0;
					result.m_Quality[Result::ST_NEG_ABS_NUM] = 0;
				}
			}
		}
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

