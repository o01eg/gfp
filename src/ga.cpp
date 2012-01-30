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

#include <iostream>
#include <sstream>
#include <fstream>
#include "ga.h"

#include "libfunctions/functions.h"

GA::GA(size_t population_size_)
	:m_PopulationSize(population_size_)
{
	VM::Environment env;
	env.LoadFunctionsFromArray(func_array);
	m_Population = new Population;
	for(size_t i = 0; i < m_PopulationSize; ++ i)
	{
		//std::clog << "Generating " << i << " individual..." << std::endl;
		m_Population->push_back(Individual::GenerateRand(env));
		//std::clog << "Generated " << i << " individual..." << std::endl;
	}
}

GA::Results GA::Examine() const
{
	Results res = Individual::Execute(*m_Population);
	if(m_PopulationSize != res.size())
	{
		return res;
	}
	for(size_t i = 0; i < m_PopulationSize; ++ i)
	{
		(*m_Population)[i].SetResult(res[i]);
	}
//	std::stringstream ss;
//	ss << "before:" << std::endl;
//	DumpResults(*m_Population, ss);
	std::stable_sort(res.begin(), res.end());
	return res;
}

bool GA::Step()
{
	Results results = Examine();
	if(m_PopulationSize != results.size())
	{
		return false;
	}

	Population *new_population = new Population;
	bool updated = false;
	{
		VM::Environment env;
		env.LoadFunctionsFromArray(func_array);

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

		while(new_population->size() < m_PopulationSize)
		{
			new_population->push_back(Individual::Crossover(env, m_Population->at(parent_pool[rand() % parent_pool.size()]), m_Population->at(parent_pool[rand() % parent_pool.size()])));
		}

		for(size_t i = 1; i < m_PopulationSize; ++ i)
		{
			if(rand() % 5 == 0)
			{
				new_population->at(i) = Individual::Mutation(env, new_population->at(i));
			}
		}

	}
	delete m_Population;
	m_Population = new_population;
	return updated;
}

void GA::DumpResults(const Population &population, std::ostream& os)
{
	for(size_t i = 0; i < population.size(); ++ i)
	{
		population[i].GetResult().Dump(os);
	}
}

void GA::Save(const char *filename)
{
	std::ofstream f(filename);
	f << GetInd(0).GetText();
}

