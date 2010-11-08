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
#include <glibmm/thread.h>
#include <csignal>
#include <string>
#include "ga.h"

/// \brief Maximum step for persistent best individuals.
const size_t MAX_STEP_UNCHANGED = 256;

bool app_is_run = true; ///< Application's state variable.

/// Handler of Ctrl+D event.
void close_handler()
{
	while(! std::cin.eof())
	{
		char c;
		std::cin >> c;
	}
	app_is_run = false;
}

/// \brief Handler of Ctrl+C event.
/// \param signum Number of signal.
void interrupt_handler(int signum)
{
	app_is_run = false;
}

/// \brief Main Function.
/// \param argc Number of arguments.
/// \param argv Array of arguments.
/// \return Exit code.
int main(int argc, char **argv)
{
#if 0
	signal(SIGINT, interrupt_handler);
#endif

	Glib::thread_init();
#if 0
	Glib::Thread::create(sigc::ptr_fun(close_handler), false);
#endif

	srand(time(0));
	
	// Start application.
	std::clog << "Start application" << std::endl;
	
	try
	{
#if 0
		VM::Environment env;
		env.LoadFunctions(DATA_DIR "functions.txt");
		try
		{
			VM::Program prog(env, DATA_DIR "2.lsp");
			env.program = &prog;
			const size_t MAX_CIRCLES = 1000;
			VM::Object a(env);
			std::cin >> a;
			env.circle_count = MAX_CIRCLES;
			std::cout << "result = " << env.Run(a) << std::endl;
			std::cout << "use " << (MAX_CIRCLES - env.circle_count) << " circles" << std::endl;
		}
		catch(Glib::Error &e)
		{
			std::clog << "Catch Glib::Error in environment: " << e.what() << std::endl;
		}
#endif
		// Genetic programming
		GA ga(18);
		std::cout << "Start evolution" << std::endl;
		size_t remain_steps = MAX_STEP_UNCHANGED;
		std::vector<GA::Operation> operations;
		operations.push_back(GA::Mutation(0));
		operations.push_back(GA::Mutation(1));
		operations.push_back(GA::Mutation(2));
		operations.push_back(GA::Mutation(3));
		operations.push_back(GA::Mutation(4));
		operations.push_back(GA::Crossover(0, 1));
		operations.push_back(GA::Crossover(0, 1));
		operations.push_back(GA::Crossover(0, 2));
		operations.push_back(GA::Crossover(0, 3));
		operations.push_back(GA::Crossover(0, 4));
		operations.push_back(GA::Crossover(1, 2));
		operations.push_back(GA::Crossover(1, 3));
		operations.push_back(GA::Crossover(2, 3));
		while(remain_steps > 0)
		{
			if(ga.Step(operations))
			{
				std::clog << "Better." << std::endl;
				remain_steps = MAX_STEP_UNCHANGED;
				std::clog << "best = " << ga.GetBest().GetText() << std::endl;
				std::clog << "result = " << ga.GetBest().GetResult().m_Result;
				std::clog << "result[GOOD_MOVES] =" << ga.GetBest().GetResult().m_Quality[Individual::Result::ST_GOOD_MOVES] << std::endl;
				std::clog << "result[BAD_MOVES] =" << ga.GetBest().GetResult().m_Quality[Individual::Result::ST_BAD_MOVES] << std::endl;
				std::clog << "result[ANSWER_QUALITY] =" << ga.GetBest().GetResult().m_Quality[Individual::Result::ST_ANSWER_QUALITY] << std::endl;
				std::clog << "result[STATE_CHANGES] =" << ga.GetBest().GetResult().m_Quality[Individual::Result::ST_STATE_CHANGES] << std::endl;
				std::clog << "result[NEG_CIRCLES] =" << ga.GetBest().GetResult().m_Quality[Individual::Result::ST_NEG_CIRCLES] << std::endl;
			}
			else
			{
				remain_steps --;
				//std::cout << "steps: " << remain_steps << std::endl;
			}
		}
		std::clog << "best = " << ga.GetBest().GetText() << std::endl;
	}
	catch(std::exception &e)
	{
		std::clog << "Catch std::exception: " << e.what() << std::endl;
	}
	catch(Glib::Error &e)
	{
		std::clog << "Catch Glib::Error: " << e.what() << std::endl;
	}
	
	// Finish application.
	std::clog << "Finish application" << std::endl;

	return 0;
}

