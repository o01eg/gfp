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

#include <algorithm>
#include "world.h"
#include "vm/ioobject.h"
#include "object_utils.h"

WorldFile World::s_File;

World::World(VM::Environment &env, const char *filename)
	:m_Map(env),
	m_CurrentMap(env),
	m_IndObject(env)
{
	if(! s_File.SetFile(filename))
	{
		std::cerr << "Cann't load world file" << std::endl;
		abort();
		return;
	}
	for(int hm = s_File.GetHeight() - 1; hm >= 0; -- hm)
	{
		VM::Object line(env);
		for(int wm = s_File.GetWidth() - 1; wm >= 0; -- wm)
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
				m_Area.push_back(std::make_pair(m_PosX, m_PosY));
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
	if(std::find(m_Area.begin(), m_Area.end(), std::make_pair(m_PosX, m_PosY)) == m_Area.end())
	{
		m_Area.push_back(std::make_pair(m_PosX, m_PosY));		
	}
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
	for(int hm = s_File.GetHeight() - 1; hm >= 0; -- hm)
	{
		VM::Object line(env);
		for(int wm = s_File.GetWidth() - 1; wm >= 0; -- wm)
		{
			if(hm || wm)
			{
				line = VM::Object(env.GetERROR(), line);
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
	for(int hm = s_File.GetHeight() - 1; hm >= 0; -- hm)
	{
		res = VM::Object(env.GetERROR(), res);
	}
	return res;
}

std::ostream& operator<<(std::ostream& os, const World& world)
{
	for(int hm = -1; hm <= static_cast<int>(world.s_File.GetHeight()); ++ hm)
	{
		for(int wm = -1; wm <= static_cast<int>(world.s_File.GetWidth()); ++ wm)
		{
			if(world.CheckCell(wm, hm))
			{
				os << "#";
			}
			else
			{
				if((wm == world.m_PosX) && (hm == world.m_PosY))
				{
					os << "@";
				}
				else
				{
					if(std::find(world.m_Area.begin(), world.m_Area.end(), std::make_pair(wm, hm)) == world.m_Area.end())
					{
						os << ".";
					}
					else
					{
						os << "+";
					}
				}
			}
		}
		os << std::endl;
	}	
	return os;
}

