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
#include <fstream>
#include "ga.h"

#if COMPILE_STATIC
#include "libfunctions/functions.h"
#endif

GA::GA(size_t population_size_)
	:m_PopulationSize(population_size_)
{
	VM::Environment env;
#if COMPILE_STATIC
	env.LoadFunctionsFromArray(func_array);
#else
	env.LoadFunctionsFromFile(DATA_DIR "functions.txt");
#endif
	m_Population = new Population;
	for(size_t i = 0; i < m_PopulationSize; i ++)
	{
		//std::clog << "Generating " << i << " individual..." << std::endl;
		m_Population->push_back(Individual::GenerateRand(env));
		//std::clog << "Generated " << i << " individual..." << std::endl;
	}
}

bool GA::Step(const std::vector<Operation> &operations)
{
	Results results = Individual::Execute(*m_Population);
	if(m_PopulationSize != results.size())
	{
		THROW("Different sizes between population and results of individuals.");
	}
	for(size_t i = 0; i < m_PopulationSize; i++)
	{
		(*m_Population)[i].SetResult(results[i]);
	}
//	std::stringstream ss;
//	ss << "before:" << std::endl;
//	DumpResults(*m_Population, ss);
	std::stable_sort(results.begin(), results.end());
	
	std::vector<Operation>::const_iterator operation;
	Results::iterator result;
	Population *new_population = new Population;
	bool updated = false;
	try //added here to avoid memory leak with new_population
	{
		VM::Environment env;
#if COMPILE_STATIC
		env.LoadFunctionsFromArray(func_array);
#else
		env.LoadFunctionsFromFile(DATA_DIR "functions.txt");
#endif
		// Add best individuals.
		size_t num_best = m_Population->size() - operations.size();
		size_t index = num_best;
		for(result = results.begin(); index; index --, result++)
		{
			new_population->push_back(m_Population->at(result->GetIndex()));
		}
		if((results[0].GetIndex() != 0) || (results[1].GetIndex() != 1))
		{
			std::clog << "  --==--" << std::endl;
			std::clog << "0 is " << results[0].GetIndex() << std::endl;
			std::clog << "1 is " << results[1].GetIndex() << std::endl;
			std::clog << "2 is " << results[2].GetIndex() << std::endl;
			std::clog << "3 is " << results[3].GetIndex() << std::endl;
			std::clog << "4 is " << results[4].GetIndex() << std::endl;
//			ss << "after:" << std::endl;
//			DumpResults(*new_population, ss);
			updated = true;
//			std::cout << ss.str();
		}
		// Make genetic operations.
		for(operation = operations.begin(); operation < operations.end();
			operation ++)
		{
			if(operation->first == -1)
			{
				// Mutation.
				new_population->push_back(Individual::Mutation(env, m_Population->at(results[operation->second].GetIndex())));
			}
			else
			{
				// Crossover.
				new_population->push_back(Individual::Crossover(env, m_Population->at(results[operation->first].GetIndex()), m_Population->at(results[operation->second].GetIndex())));
			}
		}
	}
	catch(...)
	{
		delete new_population;
		throw;
	}
	delete m_Population;
	m_Population = new_population;
	return updated;
}

void GA::DumpResults(const Population &population, std::ostream& os)
{
	for(size_t i = 0; i < population.size(); i ++)
	{
		population[i].GetResult().Dump(os);
	}
}

void GA::Save(const char *filename)
{
	std::ofstream f(filename);
	f << GetBest().GetText();
}

