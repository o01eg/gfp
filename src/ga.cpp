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

bool GA::Step()
{
	Results results = Individual::Execute(*m_Population);
	if(m_PopulationSize != results.size())
	{
		return false;
	}
	for(size_t i = 0; i < m_PopulationSize; i++)
	{
		(*m_Population)[i].SetResult(results[i]);
	}
//	std::stringstream ss;
//	ss << "before:" << std::endl;
//	DumpResults(*m_Population, ss);
	std::stable_sort(results.begin(), results.end());
	
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

		// add elite individual
		new_population->push_back(m_Population->at(results[0].GetIndex()));
		if(results[0].GetIndex() != 0)
		{
			updated = true;
		}

		// make parent pool by tournament
		std::vector<size_t> parent_pool;
		while(parent_pool.size() < m_PopulationSize - 1)
		{
			size_t i1 = rand() % results.size();
			size_t i2 = rand() % results.size();
			parent_pool.push_back(std::min(results[i1], results[i2]).GetIndex());
		}

		//make new population
		while(new_population->size() < m_PopulationSize)
		{
			switch(rand() % 3)
			{
			case 0:
				new_population->push_back(m_Population->at(parent_pool[rand() % parent_pool.size()]));
				break;
			case 1:
				// mutation
				new_population->push_back(Individual::Mutation(env, m_Population->at(parent_pool[rand() % parent_pool.size()])));
				break;
			case 2:
				// crossover
				new_population->push_back(Individual::Crossover(env, m_Population->at(parent_pool[rand() % parent_pool.size()]), m_Population->at(parent_pool[rand() % parent_pool.size()])));
				break;
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

