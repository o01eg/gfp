#ifndef _GA_H_
#define _GA_H_

#include <algorithm>
#include "individual.h"

/// \class GA
/// \brief Genetic Algoritms.
/// \note operator< for Individual::Result use "less" as better
class GA
{
	typedef std::vector<Individual> Population;
	typedef std::vector<Individual::Result> Results;
public:
	/// \brief Genetic operation.
	/// If first is -1 then it's mutation.
	/// Numbers mean index of parent.
	typedef std::pair<int, int> Operation;
	
	
	/// \brief GA constructor.
	/// @param population_size_ Size of population.
	GA(size_t population_size_)
		:population_size(population_size_)
	{
		VM::Environment env;
		population = new Population;
		for(size_t i = 0; i < population_size; i ++)
		{
			population->push_back(Individual::GenerateRand(env));
		}
	}
	
	~GA() ///< GA Desctructor
	{
		delete population;
	}

	/// \brief Step.
	/// \param operations Rules of creating new generation.
	/// \return True if best individuals changed.
	bool Step(const std::vector<Operation> &operations)
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
	
	/// \brief Save population
	void Save()
	{
		/// \todo Write this.
	}
	
	/// \brief Load population
	void Load()
	{
		/// \todo Write this.
	}

	/// \brief Mutation.
	/// @param i Index of individual.
	/// @return Genetic operation.
	static Operation Mutation(int i)
	{
		return Operation(-1, i);
	}

	/// \brief Crossover.
	/// @param i Index of first individual.
	/// @param j Index of first individual.
	/// @return Genetic operation.
	static Operation Crossover(int i, int j)
	{
		return Operation(i, j);
	}
private:
	size_t population_size; ///< Size of population.
	Population *population; ///< Auto pointer to population.
};

#endif
