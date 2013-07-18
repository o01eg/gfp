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

#include "vm/object.h"
#include "world_file.h"

/// \brief Class is describing world for individuals.
class World
{
public:
	friend std::ostream& operator<<(std::ostream& os, const World& world);

	/// \brief Construct world from file.
	/// \param[in] env Environment.
	World(VM::Environment &env);

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
	VM::Object GetCurrentWorld() const
	{
		return m_CurrentMap;
	}

	/// \brief Get world from ERROR for static check.
	/// \param[in] env Environment.
	/// \return World object.
	VM::Object GetErrorWorld(VM::Environment &env) const;

	/// \brief Get world's lines from ERROR for static check.
	/// \param[in] env Environment.
	/// \return World object.
	VM::Object GetErrorWorldLines(VM::Environment &env) const;

	/// \brief Get area size.
	/// \return Area size.
	size_t GetAreaSize() const
	{
		return m_Area.size();
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
	std::vector<std::pair<int, int> > m_Area; ///< Area of path.
};

std::ostream& operator<<(std::ostream& os, const World& world);

