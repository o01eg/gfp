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

