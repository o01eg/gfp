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
	void SetFile(const char* filename);

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

