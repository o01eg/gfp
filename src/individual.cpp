#include <iostream>
#include <sstream>
#include "individual.h"
#include "ioobject.h"
#include "ga_utils.h"

const size_t MAX_FUNCTIONS = 256;

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
	/// \todo Write this.
	return Individual(ind);
}

Individual Individual::Crossover(VM::Environment &env, const Individual& ind1, const Individual& ind2)
{
	/// \todo Write this.
	return Individual(ind1);
}

std::vector<Individual::Result> Individual::Execute(const std::vector<Individual> &population)
{
	std::vector<Individual::Result> res;
	/// \todo Write this.
	return res;
}

