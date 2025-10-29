#include "PyList.hpp"

PyList::PyList(void) : DynaList()
{
	this->m_contents = NULL;
}

bool PyList::Reallocate(int _size)
{
	this->m_len = _size;
	void* allocatedRAM = realloc(m_contents, sizeof(PyAny) * (this->m_len));
	if (allocatedRAM != NULL)
	{
		this->m_contents = (PyAny*)allocatedRAM;
		return true;
	}
	this->m_contents = NULL;
	return false;
}

int PyList::Append(PyAny _element)
{
	if (this->Reallocate(this->m_len + 1))
	{
		this->m_contents[this->m_len - 1] = _element;
		return m_len - 1;
	}
	return -1;
}

void PyList::Insert(PyAny _element, int _index)
{
	for (int i = this->Append(_element); i > _index; i--)
	{
		this->Swap(i, i - 1);
	}
}
