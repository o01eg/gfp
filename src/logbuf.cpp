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
#include <cstdlib>
#include "logbuf.h"

LogBuf *LogBuf::log_buffer = new LogBuf();

LogBuf::LogBuf()
{
	backup = std::clog.rdbuf();
	std::clog.rdbuf(this);

	atexit(DestroyLogBuf);
}

LogBuf::~LogBuf()
{
	std::cout.flush();
	std::clog.rdbuf(backup);
}

int LogBuf::overflow(int c)
{
	if(c != -1)
	{
		char ch = c;
		//std::cout << "Overflow ";
		std::cout.write(&ch, 1);
	}
	return traits_type::not_eof(c);
}

int LogBuf::sync()
{
	//std::cout << "Flush" << std::endl;
	std::cout.flush();
	return 0;
}

std::streamsize LogBuf::xsputn(const char *s, std::streamsize n)
{
	//std::cout << "Out n=" << n << " s=\"";
	std::cout.write(s, n);
	//std::cout << "\"" << std::endl;
	return n;
}

void LogBuf::DestroyLogBuf() ///< Callback for desctructor.
{
	delete log_buffer;
}


