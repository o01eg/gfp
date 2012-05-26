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
	LogBuf(const LogBuf&) = delete; //Prevent copy-constructor.
	LogBuf(LogBuf&&) = delete; //Prevent move-constructor.
	LogBuf& operator=(const LogBuf&) = delete; //Prevent assign.
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

