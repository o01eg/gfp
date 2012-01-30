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

