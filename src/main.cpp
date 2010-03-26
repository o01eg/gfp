#include <iostream>
#include <glibmm/thread.h>
#include <csignal>
#include <string>
#include "shared.h"
#include "logbuf.h"
#include "ga.h"
#include "gfp_handler.h"
#include "object.h"
#include "environment.h"
#include "ioobject.h"
#include "program.h"

/// \brief Maximum step for persistent best individuals.
const size_t MAX_STEP_UNCHANGED = 20;

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
	//signal(SIGINT, interrupt_handler);

	Glib::thread_init();
	//Glib::Thread::create(sigc::ptr_fun(close_handler), false);

	srand(time(0));
	
	// Start application.
	std::clog << "Start application" << std::endl;
	
	/*GFP_Handler *gfp_handler = new GFP_Handler;
	GA<GFP_Handler> *ga = new GA<GFP_Handler>(gfp_handler, 10);
	
	std::vector< GA<GFP_Handler>::Operation > operations;
	operations.push_back(GA<GFP_Handler>::Mutation(0));
	operations.push_back(GA<GFP_Handler>::Mutation(0));
	operations.push_back(GA<GFP_Handler>::Mutation(0));
	operations.push_back(GA<GFP_Handler>::Mutation(0));
	operations.push_back(GA<GFP_Handler>::Crossover(0, 1));
	operations.push_back(GA<GFP_Handler>::Crossover(0, 2));
	
	size_t num_no_changed = 0;
	while(app_is_run)
	{
		std::clog << "Step." << std::endl;
		if(ga->Step(operations))
		{
			// Best individuals were changed.
			num_no_changed = 0;
		}
		else
		{
			// Best individual were not changed.
			num_no_changed ++;
			if(num_no_changed > MAX_STEP_UNCHANGED)
			{
				app_is_run = false;
			}
		}
	}
	
	delete ga;
	delete gfp_handler;*/

	try
	{
		VM::Environment env;
		env.LoadFunctions(DATA_DIR "functions.txt");
		try
		{
			VM::Program prog(env, DATA_DIR "1.lsp");
			env.program = &prog;
			const size_t MAX_CIRCLES = 100;
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

