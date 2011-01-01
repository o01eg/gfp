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

#include <stack>
#include "world.h"
#include "ioobject.h"
#include "object_utils.h"

WorldFile World::s_File;

World::World(VM::Environment &env, const char *filename)
	:m_Map(env),
	m_CurrentMap(env),
	m_IndObject(env)
{
	s_File.SetFile(filename);
	for(int hm = s_File.GetHeight() - 1; hm >= 0; hm --)
	{
		VM::Object line(env);
		for(int wm = s_File.GetWidth() - 1; wm >= 0; wm --)
		{
			VM::Object obj(env);
			switch(s_File.GetMap()[hm][wm])
			{
			case '#':
				obj = VM::Object(VM::Object(env, VM::INTEGER, 0), VM::Object(env));
				break;
			case '@':
				// Get initial position.
				m_PosX = wm;
				m_PosY = hm;
				break;
			}
			line = VM::Object(obj, line);
		}
		m_Map = VM::Object(line, m_Map);
	}
	m_IndObject = VM::Object(VM::Object(env, VM::INTEGER, -1), VM::Object(env));
	UpdateCurrentMap();
}

bool World::CheckCell(int x, int y) const
{
	if((x < 0) || (x >= static_cast<int>(s_File.GetWidth())) || (y < 0) || (y >= static_cast<int>(s_File.GetHeight())))
	{
		return true;
	}
	if(s_File.GetMap()[y][x] == '#')
	{
		return true;
	}
	return false;
}

bool World::Move(int dir)
{
	int dX = 0;
	int dY = 0;
	switch(dir)
	{
	case 1: // up
		dY = -1;
		break;
	case 2: // right
		dX = +1;
		break;
	case 3: // down
		dY = +1;
		break;
	case 4: //left
		dX = -1;
		break;
	default:
		return false;
	}
	if(CheckCell(m_PosX + dX, m_PosY + dY))
	{
		return false;
	}
	m_PosX += dX;
	m_PosY += dY;
	UpdateCurrentMap();
	return true;
}

void World::UpdateCurrentMap()
{
	int y = m_PosY;
	std::stack<VM::Object> obj_stack;
	VM::Object p(m_Map);
	while((y > 0) && (! p.IsNIL()))
	{
		obj_stack.push(p.GetHead());
		p = p.GetTail();
		y --;
	}
	m_CurrentMap = VM::Object(EditList(p.GetHead(), m_PosX, m_IndObject), p.GetTail());
	while(! obj_stack.empty())
	{
		m_CurrentMap = VM::Object(obj_stack.top(), m_CurrentMap);
		obj_stack.pop();
	}
}

VM::Object World::GetErrorWorld(VM::Environment& env) const
{
	VM::Object res(env);
	for(int hm = s_File.GetHeight() - 1; hm >= 0; hm --)
	{
		VM::Object line(env);
		for(int wm = s_File.GetWidth() - 1; wm >= 0; wm --)
		{
			if(hm || wm)
			{
				line = VM::Object(VM::Object(env, VM::ERROR), line);
			}
			else
			{
				// avoid simple "(CAR (CAR (CAR $)))" solve.
				line = VM::Object(VM::Object(env), line);
			}
		}
		res = VM::Object(line, res);
	}
	return res;
}

VM::Object World::GetErrorWorldLines(VM::Environment &env) const
{
	VM::Object res(env);
	for(int hm = s_File.GetHeight() - 1; hm >= 0; hm --)
	{
		res = VM::Object(VM::Object(env, VM::ERROR), res);
	}
	return res;
}

