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
#include <csignal>
#include <string>
#include <fstream>
#include <ctime>
//#include <thread>

#include "ga.h"
#include "current_state.h"
#include "conf.h"

/// \brief Maximum step for persistent best individuals.
const size_t MAX_STEP_UNCHANGED = Config::Instance().GetSLong("max-step-unchanged", 100);

/// Handler of Ctrl+D event.
void close_handler()
{
	while(! std::cin.eof())
	{
		char c;
		std::cin >> c;
	}
	CurrentState::Shutdown();
	VM::Environment::Stop();
}

/// \brief Handler of Ctrl+C event.
/// \param signum Number of signal.
void interrupt_handler(int signum)
{
	CurrentState::Dump();
	CurrentState::Shutdown();
	VM::Environment::Stop();
}

/// \brief Main Function.
/// \param argc Number of arguments.
/// \param argv Array of arguments.
/// \return Exit code.
int main(int argc, char **argv)
{
#if 1
	signal(SIGINT, interrupt_handler);
#endif

#if 0
	std::thread thr(close_handler);
#endif

	time_t seed = time(NULL);
	std::cout << "seed = " << seed << std::endl;
	srand(static_cast<unsigned int>(seed));

	char *filename = NULL;
	if(argc == 2)
	{
		filename = argv[1];
	}
	
	// Start application.
	std::clog << "Start application" << std::endl;
	
		// Genetic programming
		GA ga(Config::Instance().GetSLong("population-size", 10));
		if(filename)
		{
			if(! ga.Load(filename))
			{
				std::cout << "Cann't load " << filename << std::endl;
			}
		}
		std::cout << "Start evolution" << std::endl;
		size_t remain_steps = MAX_STEP_UNCHANGED;
		size_t generation = 0;

		while((remain_steps > 0) && CurrentState::IsRun())
		{
			generation ++;
			CurrentState::s_Generation = generation;
			if(ga.Step())
			{
				std::clog << std::endl << "Better." << std::endl;
				remain_steps = MAX_STEP_UNCHANGED;
				for(std::vector<Individual>::const_iterator parent = ga.GetInd(0).GetParents().begin(); parent != ga.GetInd(0).GetParents().end(); ++ parent)
				{
					std::clog << "parent = " << parent->GetText() << std::endl;
					parent->GetResult().Dump(std::clog);
					std::clog << "   =====" << std::endl;
				}
				std::clog << "1st = " << ga.GetInd(0).GetText() << std::endl;
				ga.GetInd(0).GetResult().Dump(std::clog);
				std::clog << "generation = " << generation << std::endl << std::endl;
			}
			else
			{
				remain_steps --;
				//std::cout << "steps: " << remain_steps << std::endl;
			}
			if(generation % 10 == 0)
			{
				std::cout << "\x1b[0`generation = " << generation << "(" << (remain_steps * 100 / MAX_STEP_UNCHANGED) << "%)            ";
				std::cout.flush();
			}
		}
		GA::Results results = ga.Examine();
		std::ofstream f("population.txt");
		for(GA::Results::const_iterator it = results.begin(); it != results.end(); ++ it)
		{
			f << "[" << size_t(it - results.begin()) << "] = " << ga.GetInd(it->GetIndex()).GetText() << std::endl;
			ga.GetInd(it->GetIndex()).GetResult().Dump(f);
		}
		if(filename)
		{
			ga.Save(filename);
		}

	// Finish application.
	std::clog << "Finish application" << std::endl;
#if 0
	std::cin.setstate(std::ios_base::eofbit);
	thr.join();
#endif

	return 0;
}

