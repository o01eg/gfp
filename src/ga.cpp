#include "ga.h"

GA::GA(size_t population_size_)
	:population_size(population_size_)
{
	VM::Environment env;
	population = new Population;
	for(size_t i = 0; i < population_size; i ++)
	{
		population->push_back(Individual::GenerateRand(env));
	}
}

bool GA::Step(const std::vector<Operation> &operations)
{
	Results results = Individual::Execute(*population);
	std::sort(results.begin(), results.end());
	
	std::vector<Operation>::const_iterator operation;
	Results::iterator result;
	Population *new_population = new Population;
	bool updated = false;
	try //added here to avoid memory leak with new_population
	{
		VM::Environment env;
		// Add best individuals.
		size_t num_best = population->size() - operations.size();
		size_t index = num_best;
		for(result = results.begin(); index; index --, result++)
		{
			new_population->push_back(population->at(result->GetIndex()));
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
				new_population->push_back(Individual::Mutation(env, population->at(results[operation->second].GetIndex())));
			}
			else
			{
				// Crossover.
				new_population->push_back(Individual::Crossover(env, population->at(results[operation->first].GetIndex()), population->at(results[operation->second].GetIndex())));
			}
		}
	}
	catch(...)
	{
		delete new_population;
		throw;
	}
	delete population;
	population = new_population;
	return updated;
}

