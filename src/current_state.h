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

#ifndef _CURRENT_STATE_H_
#define _CURRENT_STATE_H_

#include <string>
#include <iostream>
#include <assert.h>

/// \brief Save current state of evalation
class CurrentState
{
public:
	/// \brief Dump current state
	static void Dump()
	{
		std::cerr << "Dump state:" << std::endl;
		std::cerr << " Generation: " << s_Generation << std::endl;
		std::cerr << " Program Text: " << s_Program << std::endl;
		std::cerr << std::endl;
	}

	/// \brief Shutdown program
	static void Shutdown()
	{
		s_AppIsRun = false;
	}

	/// \brief Check if program run.
	/// \return Is program run.
	static bool IsRun()
	{
		return s_AppIsRun;
	}

	static std::string s_Program; ///< Current evalating program.
	static size_t s_Generation; ///< Current generation.
private:
	static bool s_AppIsRun; ///< Run flag.
};

#endif

