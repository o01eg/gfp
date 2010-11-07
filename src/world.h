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

#include "object.h"
#include "world_file.h"

/// \brief Class is describing world for individuals.
class World
{
public:
	/// \brief Construct world from file.
	/// \param[in] env Environment.
	/// \param[in] filename Name of file with world.
	World(VM::Environment &env, const char* filename);

	/// \brief Desctruct world.
	~World()
	{
	}

	/// \brief Move individual in world.
	/// \param[in] dir Direction.
	/// \return true if success.
	bool Move(int dir);

	/// \brief Is wall in cell?
	/// \param[in] x X coordinate.
	/// \param[in] y Y coordiante.
	/// \return Is wall here.
	bool CheckCell(int x, int y) const;

	/// \brief Get current world.
	/// \return Current world.
	VM::WeakObject GetCurrentWorld() const
	{
		return m_CurrentMap;
	}
private:
	/// \brief Update current map.
	void UpdateCurrentMap();

	int m_PosX; ///< Current X position of individual.
	int m_PosY; ///< Current Y position of individual.
	static WorldFile s_File; ///< Context of world file.
	VM::Object m_Map; ///< Static map;
	VM::Object m_CurrentMap; ///< Cureent map with individual.
	VM::Object m_IndObject; ///< Individual as object.
};

