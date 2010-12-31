/*
 * Copyright (C) 2010-2011 O01eg <o01eg@yandex.ru> 
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

#ifndef _LOGBUF_H_
#define _LOGBUF_H_

#include <streambuf>

/// \class LogBuf
/// \brief Stream Buffer for clog.
/// \todo Append Multi-threading support.
/// \todo Append buffering logs.
/// \todo Append catch signals.
class LogBuf: public std::streambuf
{
private:
	LogBuf(); ///< Default constructor.
	LogBuf(const LogBuf&); ///< Avoid autogenerating public constructor.
	LogBuf& operator=(const LogBuf&); ///< Avoid autogenerating public operator.
	~LogBuf(); ///< Destructor.

	/// \brief Overloaded std::streambuf::overflow.
	/// Out symbol.
	/// @param c Outputed symbol.
	/// @return EOF if fail, other if success.
	int overflow(int c);

	/// \brief Overloaded std::streambuf::sync.
	/// Flush stream.
	/// @return Zero if success, other if fail.
	int sync();

	/// \brief Overloaded std::streambuf::xsputn.
	/// Out sequence of symbols.
	/// @param s Sequence of symbols.
	/// @param n Size of sequence.
	/// @return Number of written symbols.
	std::streamsize xsputn(const char *s, std::streamsize n);

	static void DestroyLogBuf(); ///< Callback for finish application.

	static LogBuf *log_buffer; ///< The one instance of LogBuf.

	std::streambuf *backup; ///< Previous std::clog buffer.
	int fd; ///< Descriptor of log file.
};

#endif

