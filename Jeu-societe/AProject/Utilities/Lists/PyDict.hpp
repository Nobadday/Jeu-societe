#pragma once

#ifndef _INC_PYDICT
#define _INC_PYDICT

// C Libs
#include <malloc.h>
#include <string>

#include "DynaList.hpp"


class PyDictItem
{
	private:
		std::string m_key;
		void* m_value;

	public:
		PyDictItem(const char* _key, void* _value = NULL);
		PyDictItem(const std::string& _key, void* _value = NULL);

		void SetKey(const char* _key);
		void SetKey(const std::string& _key);

		void SetValue(void* _value);
		const char* GetKey(void);
		const std::string& GetKeyString(void);
		void* GetValue(void);
		void*& GetValueReference(void);
};


class PyDict
{
	private:
		// Dynamic list containing PyDictItem
		DynaList m_contents;
	
	public:
		PyDict(void);
		PyDict(PyDict& _copy);


		// Returns the length of the dict
		int GetLen(void);
		// Returns the length of the dict
		int Len(void);

		PyDictItem* GetItem(int _index);

		// Gets the key of a item at the specified index
		const char* GetKey(int _index);
		const std::string& GetKeyString(int _index);
		
		// Get the value with index
		void* GetValue(int _index);
		void*& GetValueReference(int _index);

		

		// Get the index of the key
		// -1 if not found
		int FindKeyIndex(const char* _key);
		int FindKeyIndex(const std::string& _key);

		// Returns true if the key exists
		bool KeyExists(const char* _key);
		bool KeyExists(const std::string& _key);

		// Returns true if the index is valid
		bool IndexExists(int _index);

		// Get the value of a key
		void* Get(const char* _key);
		void* GetOrDefault(const char* _key, void* _default);

		void* GetSecured(const char* _key);
		void*& GetReference(const char* _key);


		// Add/modify an item into the dictionnary
		int Update(const char* _key, void* _value);


		void RemoveIndex(int _index);


		void* PopIndex(int _index);

		// Removes and returns the value of the key
		void* Pop(const char* _key);

		void Clear(void);

		void Print(void);

		void*& operator[](const char* _key);
		void*& operator[](int _index);
		void operator+=(PyDictItem _item);


		~PyDict(void);
};


#endif
// Python-like Dictionnary C++ : PyDict v1.0