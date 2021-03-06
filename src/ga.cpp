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

#include <fstream>
#include <sstream>
#include "ga.h"
#include "conf.h"
#include "ga_utils.h"
#include "functions.h"

GA::GA(size_t population_size_)
	: m_PopulationSize(population_size_)
{
	m_Funcs.push_back(std::make_pair(m_Env.GetIF(), 3));
	m_Funcs.push_back(std::make_pair(m_Env.GetEVAL(), 1));
	FuncData* array = func_array;
	while(array->func)
	{
		size_t index = m_Env.LoadFunction(array->name, array->number_param, array->func);
		VM::Object f = VM::Object(m_Env, VM::FUNC, index);
		m_Funcs.push_back(std::make_pair(f, array->number_param));
		// load optimize rules
		m_OptimizeRules.mode.insert({f, array->mode});
		m_OptimizeRules.returnERRORat.insert({f, array->returnERRORat});
		if(array->return0atBOOL)
		{
			m_OptimizeRules.return0atBOOL.insert(f);
		}

		++ array;
	}

	for(size_t i = 0; i < m_PopulationSize; ++ i)
	{
		//std::clog << "Generating " << i << " individual..." << std::endl;
		m_Population.push_back(GenerateRand());
		m_Population[i].Optimize(m_OptimizeRules);
		//std::clog << "Generated " << i << " individual..." << std::endl;
	}
}

GA::Results GA::Examine() const
{
	Results res = Individual::Execute(m_Env, m_Population);
	return res;
}

bool GA::Step()
{
	Results results = Examine();

	if(m_PopulationSize != results.size())
	{
		return false;
	}
	for(size_t i = 0; i < m_PopulationSize; ++ i)
	{
		m_Population[i].SetResult(results[i]);
	}
	std::stable_sort(results.begin(), results.end());

	Population new_population;
	bool updated = false;
	{
		// add elite individual
		new_population.push_back(m_Population[results[0].GetIndex()]);

		if(results[0].GetIndex() != 0)
		{
			updated = true;
		}

		// make parent pool by tournament
		std::vector<size_t> parent_pool;
		parent_pool.push_back(results[0].GetIndex());
		while(parent_pool.size() < m_PopulationSize)
		{
			size_t i1 = rand() % results.size();
			size_t i2 = rand() % results.size();
			parent_pool.push_back(results[std::min(i1, i2)].GetIndex());
		}

		while(new_population.size() < m_PopulationSize)
		{
			new_population.push_back(Crossover(m_Population[parent_pool[rand() % parent_pool.size()]], m_Population[parent_pool[rand() % parent_pool.size()]]));
		}

		for(size_t i = 1; i < m_PopulationSize; ++ i)
		{
			if(rand() % 4 == 0)
			{
				new_population[i] = Mutation(new_population[i]);
			}
		}

	}
	m_Population = std::move(new_population);
	for(auto& ind : m_Population)
	{
		ind.Optimize(m_OptimizeRules);
	}
	return updated;
}

void GA::DumpResults(const Population &population, std::ostream& os)
{
	for(size_t i = 0; i < population.size(); ++ i)
	{
		population[i].GetResult().Dump(os);
	}
}

Individual GA::GenerateRand() const
{
	VM::Program prog = GP::GenerateProg(m_Env, m_Funcs);
	return Individual(prog, {});
}

Individual GA::Mutation(const Individual& ind) const
{
	if(rand() % 2)
	{
		Individual(GP::SplitADFProg(ind.GetProgram()), ind.GetParents());
	}
	return Individual(GP::MutateProg(ind.GetProgram(), m_Funcs), ind.GetParents());
}

Individual GA::Crossover(const Individual& ind1, const Individual& ind2) const
{
	return Individual(GP::CrossoverProg(ind1.GetProgram(), ind2.GetProgram()), {ind1, ind2});
}

void GA::SetInd(size_t index, const std::string& str)
{
	if(index < m_PopulationSize)
	{
		VM::Object obj(m_Env);
		std::stringstream ss(str);
		ss >> obj;

		VM::Program prog(obj);
		m_Population[index] = Individual(std::move(prog), {});
		m_Population[index].Optimize(m_OptimizeRules);
	}
}

