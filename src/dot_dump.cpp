#include <fstream>
#include "dot_dump.h"
#include "object.h"

void DotDump(VM::Environment &env, const char* filename)
{
	std::ofstream f(filename);
	f << "digraph G {" << std::endl;
	VM::Object::PrintObjects(env, f);
	env.heap.CheckLeaks(f);
	f << "}" << std::endl;
}

