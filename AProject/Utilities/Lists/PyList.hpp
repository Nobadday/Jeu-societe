#pragma once

#ifndef _INC_PYLIST_HPP
#define _INC_PYLIST_HPP

#include <any>
#include "DynaList.hpp"


typedef std::any PyAny;

class PyList : public DynaList
{
	protected:
		// PyAny m_contents[]
		PyAny* m_contents;

	public:
		PyList(void);

		// Allocate _size of PyAny
		bool Reallocate(int _size);

		// Add an element at the end of the list and returns his index (-1 if it failed)
		int Append(PyAny _element);

		// Add an element at a specified index
		void Insert(PyAny _element, int _index);

};


#endif
// Python-like dynamic List C++ : PyList v1.0