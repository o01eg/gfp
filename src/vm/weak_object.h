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

#ifndef _WEAK_OBJECT_H_
#define _WEAK_OBJECT_H_

#include "environment.h"

namespace VM
{
	/// \brief Types of objects.
	enum Types
	{
		ERROR = 0,
		INTEGER,
		FUNC,
		ADF,
		PARAM,
		QUOTE,
		IF,
		LIST,
		EVAL,
		SYMBOL,
		MACRO
	};

	/// \brief Object for unchanging reference counter.
	class WeakObject
	{
	public:
		/// \brief Copy constructor.
		/// \param obj Object.
		WeakObject(const WeakObject& obj)
			:m_Env(obj.m_Env),
			m_Pos(obj.m_Pos)
		{
		}

		/// \brief Generate NIL object (it always is weak object)
		/// \param env Environment.
		explicit WeakObject(const Environment& env)
			:m_Env(env),
			m_Pos(0)
		{
		}

		/// \brief Destructor (not to do).
		~WeakObject()
		{
		}

		/// \brief Copying.
		/// \param[in] obj Right value.
		/// \return l-value weak object.
		WeakObject& operator=(const WeakObject& obj);

		/// \brief Compare.
		/// \param[in] obj Right value.
		/// \return Result of comparsion.
		bool operator==(const WeakObject& obj) const;

		/// \brief Compare.
		/// \param[in] obj Right value.
		/// \return Result of comparsion.
		bool operator!=(const WeakObject& obj) const
		{
			return (! operator==(obj));
		}

		/// \brief Check object for NIL.
		/// \return Answer.		
		bool IsNIL() const {return (! m_Pos);}

		/// \brief Get type.
		/// You must check object about NIL before call this function.
		/// \sa IsNIL
		/// \return type of object.
		Types GetType() const {return static_cast<Types>(m_Env.heap.At(m_Pos).hash & 0xf);}

		/// \brief Get value.
		/// You must check object about NIL before call this function.
		/// \sa IsNIL
		/// \return Value of object.
		Heap::UInt GetValue() const;

		/// \brief Get head of LIST
		/// \return head.
		WeakObject GetHead() const;

		/// \brief Get tail of LIST
		/// \return tail.
		WeakObject GetTail() const;

		/// \brief Get constant environment of object
		/// \return Environment.
		const Environment& GetEnv() const {return m_Env;}
	private:
		/// \brief Create object by position.
		/// \param env Environment.
		/// \param pos Position.
		/// \return Generated weak object.
		static WeakObject GetWeakObjectFrom(const Environment &env, Heap::UInt pos);

		const Environment &m_Env; ///< Current environment.
	protected:
		Heap::UInt m_Pos; ///< Position in heap.
	};
}

#endif

