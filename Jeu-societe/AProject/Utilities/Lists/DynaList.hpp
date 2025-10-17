#pragma once

#ifndef _INC_DYNALIST_HPP
#define _INC_DYNALIST_HPP

// C Libs
#include <stdio.h>
#include <malloc.h>

// Dynamic List of pointers
class DynaList
{
	protected:
		int m_len = 0;
		// void* m_contents[]
		void** m_contents = NULL;

	public:
		// Default constructor, create an empty list
		DynaList(void);
		// Copy constructor
		DynaList(DynaList& _copy);
		// Convert a C list made of void* into a DynaList
		DynaList(void* _list[], int _size);

		// Allocate _size of void*
		bool Reallocate(int _size);

		// Add an element at the end of the list and returns his index (-1 if it failed)
		int Append(void* _element);

		// Add an element at a specified index
		void Insert(void* _element, int _index);

		// Increases the size of the list with _element
		void FillWith(int _size, void* _element = NULL);
		// Increases the size of the list with NULLs
		void Fill(int _size);

		// Replace an element with the given element
		void Replace(int _index, void* _element = NULL);

		// Pop the contents of _extender into _list
		void Extend(DynaList& _extender);

		// Swap the position of two elements inside the list
		void Swap(int _indexA, int _indexB);

		// Reverse the order of all elements
		void Reverse(void);

		// Remove the element from the list at the given index
		void Remove(int _index);

		// Removes and gets the element from the list
		void* Pop(int _index);
		void* PopLast(void);

		// Removes every element from the list
		void Clear(void);

		// Gets the size of the list
		int GetLen(void);
		// Gets the size of the list
		int Len(void);

		// Get the element from the list at the given index
		void* GetElement(int _index);
		// Get the element from the list at the given index (NULL if not found)
		void* GetElementSecured(int _index);

		// Get the last element of the list
		void* GetElementLast(void);

		void*& GetElementReference(int _index);

		// Turns negative indexes into positives at the end of the list [-1] -> [(len-1)-1]
		void CorrectIndex(int* _index);
		// Turns negative indexes into positives at the end of the list [-1] -> [(len-1)-1]
		int GetCorrectIndex(int _index);

		// Returns true if the index exists inside the list
		bool IndexExists(int _index);

		// Find the index of the first occurence of the given element (-1 if not found)
		int Index(void* _element, int _start, int _end);

		// Find the index of a element (-1 if not found)
		int FindElementIndex(void* _element);

		// Returns true if _element is found inside the list
		bool ContainsElement(void* _element);

		// Count the number of occurences of an element inside the list
		int Count(void* _element);

		bool IsEmpty(void);

		void Print(void);

		void*& operator[](int _index);
		void operator+=(void* _element);
		
		~DynaList(void);
};


#endif
// Python-inspired dynamic List for C++ : DynaList v1.0