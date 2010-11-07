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
#include "ga.h"

GA::GA(size_t population_size_)
	:m_PopulationSize(population_size_)
{
	VM::Environment env;
	env.LoadFunctions(DATA_DIR "functions.txt");
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
	std::sort(results.begin(), results.end());
	
	std::vector<Operation>::const_iterator operation;
	Results::iterator result;
	Population *new_population = new Population;
	bool updated = false;
	try //added here to avoid memory leak with new_population
	{
		VM::Environment env;
		env.LoadFunctions(DATA_DIR "functions.txt");
		// Add best individuals.
		size_t num_best = m_Population->size() - operations.size();
		size_t index = num_best;
		for(result = results.begin(); index; index --, result++)
		{
			new_population->push_back(m_Population->at(result->GetIndex()));
			if(result->GetIndex() >= num_best)
			{
				updated = true;
			}
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

