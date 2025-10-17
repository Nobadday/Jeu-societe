#include "PyDict.hpp"

#pragma region PyDict Item
PyDictItem::PyDictItem(const char* _key, void* _value)
{
	this->SetKey(_key);
	this->m_value = _value;
}
PyDictItem::PyDictItem(const std::string& _key, void* _value)
{
	this->SetKey(_key);
	this->m_value = _value;
}

void PyDictItem::SetKey(const char* _key)
{
	this->m_key.assign(_key);
}
void PyDictItem::SetKey(const std::string& _key)
{
	this->m_key.assign(_key);
}

void PyDictItem::SetValue(void* _value)
{
	this->m_value = _value;
}

const char* PyDictItem::GetKey(void)
{
	return this->m_key.c_str();
}

const std::string& PyDictItem::GetKeyString(void)
{
	return this->m_key;
}

void* PyDictItem::GetValue(void)
{
	return this->m_value;
}
void*& PyDictItem::GetValueReference(void)
{
	return this->m_value;
}
#pragma endregion


PyDict::PyDict(void)
{
	// Nothing to initialise
}

PyDict::PyDict(PyDict& _copy)
{
	printf("TODO : PyDictCopy()\n");
}


int PyDict::GetLen(void)
{
	return this->m_contents.GetLen();
}
int PyDict::Len(void)
{
	return this->GetLen();
}

PyDictItem* PyDict::GetItem(int _index)
{
	return (PyDictItem*)this->m_contents.GetElement(_index);
}

const char* PyDict::GetKey(int _index)
{
	return this->GetItem(_index)->GetKey();
}

const std::string& PyDict::GetKeyString(int _index)
{
	return this->GetItem(_index)->GetKeyString();
}

void* PyDict::GetValue(int _index)
{
	return this->GetItem(_index)->GetValue();
}

void*& PyDict::GetValueReference(int _index)
{
	return this->GetItem(_index)->GetValueReference();
}

int PyDict::FindKeyIndex(const char* _key)
{
	for (int i = this->GetLen() - 1; i >= 0; i--)
	{
		if (this->GetKeyString(i).compare(_key) == 0)
		{
			return i;
		}
	}
	return -1;
}

int PyDict::FindKeyIndex(const std::string& _key)
{
	for (int i = this->GetLen() - 1; i >= 0; i--)
	{
		if (this->GetKeyString(i).compare(_key) == 0)
		{
			return i;
		}
	}
	return -1;
}

bool PyDict::KeyExists(const char* _key)
{
	return this->FindKeyIndex(_key) != -1;
}
bool PyDict::KeyExists(const std::string& _key)
{
	return this->FindKeyIndex(_key) != -1;
}


bool PyDict::IndexExists(int _index)
{
	return this->m_contents.IndexExists(_index);
}

void* PyDict::Get(const char* _key)
{
	return this->GetValue(this->FindKeyIndex(_key));
}

void* PyDict::GetOrDefault(const char* _key, void* _default)
{
	int index = this->FindKeyIndex(_key);
	if (index != -1)
	{
		return this->GetValue(index);
	}
	return _default;
}

void* PyDict::GetSecured(const char* _key)
{
	return this->GetOrDefault(_key, NULL);
}

void*& PyDict::GetReference(const char* _key)
{
	return this->GetValueReference(this->FindKeyIndex(_key));
}

int PyDict::Update(const char* _key, void* _value)
{
	int index = this->FindKeyIndex(_key);
	if (index != -1)
	{
		// Key already exists
		this->GetItem(index)->SetValue(_value);
		return index;
	}

	// Create a new item
	return this->m_contents.Append(new PyDictItem(_key, _value));
}

void PyDict::RemoveIndex(int _index)
{
	delete (PyDictItem*)this->m_contents.Pop(_index);
}

void* PyDict::PopIndex(int _index)
{
	void* _value = this->GetValue(_index);
	this->RemoveIndex(_index);
	return _value;
}

void* PyDict::Pop(const char* _key)
{
	return this->PopIndex(this->FindKeyIndex(_key));
}

void PyDict::Clear(void)
{
	for (int i = this->GetLen() - 1; i >= 0; i--)
	{
		this->RemoveIndex(i);
	}
}

void PyDict::Print(void)
{
	printf("{");
	for (int i = 0; i < this->GetLen(); i++)
	{
		printf("('%s' : <%p>)", this->GetKey(i), this->GetValue(i));
		if (i < this->GetLen() - 1)
		{
			printf(", ");
		}
	}
	printf("}\n");
}

void*& PyDict::operator[](const char* _key)
{
	int index = this->FindKeyIndex(_key);
	if (index == -1)
	{
		// Key doesnt exist, create it
		index = this->m_contents.Append(new PyDictItem(_key, NULL));
	}
	
	return this->GetValueReference(index);
	//return this->GetReference(_key);
}
void*& PyDict::operator[](int _index)
{
	return this->GetValueReference(_index);
}

void PyDict::operator+=(PyDictItem _item)
{
	this->Update(_item.GetKey(), _item.GetValue());
}

PyDict::~PyDict(void)
{
	printf("Destroy\n");
}

// Python-like Dictionnary C++ : PyDict v1.0