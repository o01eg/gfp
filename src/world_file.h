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

#ifndef _WORLD_FILE_H_
#define _WORLD_FILE_H_

#include <string>

/// \brief Class is store world's file context between using.
class WorldFile
{
public:
	/// \brief Construct world file context.
	WorldFile()
	{
		m_Map = 0;
	}

	/// \brief Desruct world file context.
	~WorldFile();

	/// \brief Change current file.
	/// \param[in] filename File with world context.
	bool SetFile(const char* filename);

	/// \brief Read width of map.
	/// \return Width.
	size_t GetWidth() const
	{
		return m_Width;
	}

	/// \brief Read height of map.
	/// \return Height.
	size_t GetHeight() const
	{
		return m_Height;
	}

	/// \brief Read map.
	/// \return Map.
	char **GetMap() const
	{
		return m_Map;
	}
private:

	WorldFile(const WorldFile&); ///< To avoid copying.
	WorldFile& operator=(const WorldFile&); ///< To avoid copying.

	char **m_Map; ///< Map of world.
	size_t m_Width; ///< Width of world.
	size_t m_Height; ///< Height of world.
	std::string m_FileName; ///< File with world.
};

#endif

