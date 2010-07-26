#include "individual.h"

Individual Individual::GenerateRand(VM::Environment &env)
{
	VM::Program prog(env);
	/// \todo Write this.
	return Individual(prog);
}

Individual Individual::Mutation(VM::Environment &env, const Individual& ind)
{
	VM::Program prog(env);
	/// \todo Write this.
	return Individual(prog);
}

Individual Individual::Crossover(VM::Environment &env, const Individual& ind1, const Individual& ind2)
{
	VM::Program prog(env);
	/// \todo Write this.
	return Individual(prog);
}

std::vector<Individual::Result> Execute(const std::vector<Individual> &population)
{
	std::vector<Individual::Result> res;
	/// \todo Write this.
	return res;
}

