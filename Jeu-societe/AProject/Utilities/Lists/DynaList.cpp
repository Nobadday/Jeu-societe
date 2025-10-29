#include "DynaList.hpp"

DynaList::DynaList(void)
{

}

DynaList::DynaList(DynaList& _copy)
{
	this->Reallocate(_copy.m_len);
	for (int i = 0; i < _copy.m_len; i++)
	{
		this->Replace(i, _copy.GetElement(i));
	}
}

DynaList::DynaList(void* _list[], int _size)
{
	this->Reallocate(_size);
	for (int i = 0; i < _size; i++)
	{
		this->Replace(i, _list[i]);
	}
}

bool DynaList::Reallocate(int _size)
{
	this->m_len = _size;
	void* allocatedRAM = realloc(m_contents, sizeof(void*) * (this->m_len));
	if (allocatedRAM != NULL)
	{
		this->m_contents = (void**)allocatedRAM;
		return true;
	}
	this->m_contents = NULL;
	return false;
}

int DynaList::Append(void* _element)
{
	if (this->Reallocate(this->m_len + 1))
	{
		this->m_contents[this->m_len - 1] = _element;
		return m_len - 1;
	}
	return -1;
}

void DynaList::Insert(void* _element, int _index)
{
	for (int i = this->Append(_element); i > _index; i--)
	{
		this->Swap(i, i - 1);
	}
}

void DynaList::FillWith(int _size, void* _element)
{
	this->Reallocate(this->m_len + _size);
	for (int i = this->m_len - _size; i < _size; i++)
	{
		this->Replace(i, _element);
	}
}

void DynaList::Fill(int _size)
{
	this->FillWith(_size, NULL);
}

void DynaList::Replace(int _index, void* _element)
{
	this->m_contents[_index] = _element;
}

void DynaList::Extend(DynaList& _extender)
{
	this->Reallocate(this->m_len + _extender.m_len);
	for (int i = _extender.m_len - 1; i >= 0; i--)
	{
		this->Replace(i, _extender.Pop(0));
	}
}

void DynaList::Swap(int _indexA, int _indexB)
{
	void* ptrTMP = this->m_contents[_indexA];
	this->m_contents[_indexA] = this->m_contents[_indexB];
	this->m_contents[_indexB] = ptrTMP;
}

void DynaList::Reverse(void)
{
	for (int i = 0; i < this->m_len / 2; i++)
	{
		this->Swap(i, (this->m_len - 1) - i);
	}
}

void DynaList::Remove(int _index)
{
	this->m_contents[_index] = NULL;
	// Sort
	for (int i = _index; i < this->m_len - 1; i++)
	{
		this->m_contents[i] = this->m_contents[i + 1];
		this->m_contents[i + 1] = NULL;
	}
	this->Reallocate(this->m_len - 1);
}

void* DynaList::Pop(int _index)
{
	void* returnValue = this->GetElement(_index);
	this->Remove( _index);
	return returnValue;
}

void* DynaList::PopLast(void)
{
	return this->Pop(this->m_len - 1);
}

void DynaList::Clear(void)
{
	this->Reallocate(0);
}

int DynaList::GetLen(void)
{
	return this->m_len;
}
int DynaList::Len(void)
{
	return this->GetLen();
}

void* DynaList::GetElement(int _index)
{
	return this->m_contents[_index];
}

void* DynaList::GetElementSecured(int _index)
{
	if (this->IndexExists(_index))
	{
		return this->GetElement(_index);
	}
	return NULL;
}

void* DynaList::GetElementLast(void)
{
	return this->GetElement(this->m_len - 1);
}

void*& DynaList::GetElementReference(int _index)
{
	return this->m_contents[_index];
}

void DynaList::CorrectIndex(int* _index)
{
	*_index = this->GetCorrectIndex(*_index);
}

int DynaList::GetCorrectIndex(int _index)
{
	if (_index < 0)
	{
		_index = this->m_len + _index;
	}
	return _index;
}

bool DynaList::IndexExists(int _index)
{
	return (_index >= 0) && (_index < this->m_len);
}

int DynaList::Index(void* _element, int _start, int _end)
{
	for (int i = _start; i < _end; i++)
	{
		if (_element == this->GetElement(i))
		{
			return i;
		}
	}
	return -1;
}

int DynaList::FindElementIndex(void* _element)
{
	return this->Index(_element, 0, this->m_len);
}

bool DynaList::ContainsElement(void* _element)
{
	return this->FindElementIndex(_element) != -1;
}

int DynaList::Count(void* _element)
{
	int occurences = 0;
	for (int i = this->m_len - 1; i >= 0; i--)
	{
		occurences += (this->GetElement(i) == _element);
	}
	return occurences;
}

bool DynaList::IsEmpty(void)
{
	return this->m_len <= 0;
}

void DynaList::Print(void)
{
	printf("[");
	for (int i = 0; i < this->m_len; i++)
	{
		if (this->GetElement(i) == NULL)
		{
			printf("_");
		}
		else
		{
			printf("X");
		}

		if (i < this->m_len - 1)
		{
			printf(", ");
		}
	}
	printf("]\n");
}

void*& DynaList::operator[](int _index)
{
	return this->GetElementReference(_index);
}


void DynaList::operator+=(void* _element)
{
	this->Append(_element);
}


DynaList::~DynaList(void)
{
	this->Clear();
}