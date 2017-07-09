
#include "Base_array.h"
#include<iostream>
#include<stdexcept>

//default constructor
template<typename T>
Base_array<T>::Base_array (void)
	:data_(new T[10])
	,cur_size_(0)
	,max_size_(10)
{

}

template<typename T>
Base_array<T>::Base_array (size_t length)
	:data_(new T[length])
	,cur_size_(length)
	,max_size_(length)
{

}

template<typename T>
Base_array<T>::Base_array (size_t length, T fill)
	:data_(new T[length])
	,cur_size_(length)
	,max_size_(length)
{
	//Assigning each element with desired character
	for (int i = 0; i < cur_size_; i++)
		data_[i] = fill;
}

template <typename T>
Base_array<T>::Base_array (const Base_array & array)
	:data_(new T[array.max_size_])
	,cur_size_(array.cur_size_)
	,max_size_(array.max_size_)
{
	//copy the value
	for (int i = 0; i < max_size_; i++)
		data_[i] = array.data_[i];
}

template <typename T>
Base_array<T>::~Base_array (void)
{
	delete[] data_;
}

template <typename T>
const Base_array<T> & Base_array<T>::operator = (const Base_array & rhs)
{
	//Checking for self assignment
	if(this == &rhs)
		return *this;

	//If the max sizes are not equal then make lhs == rhs
	if(max_size_ != rhs.max_size_){
		delete[] data_;
		data_ = new T[rhs.max_size_];
		cur_size_ = rhs.cur_size_;
		max_size_ = rhs.max_size_;
	}
	for (int i = 0; i < max_size_; i++)
		data_[i] = rhs.data_[i];

	return *this;
}

template <typename T>
T & Base_array<T>::operator [] (size_t index)
{
	if (index >= cur_size_)
		throw std::out_of_range("\aOut of range error: subscript method");
	else
		return data_[index];
}

template <typename T>
const T & Base_array<T>::operator [] (size_t index) const
{
	if (index >= cur_size_)
		throw std::out_of_range("\aOut of range error: subscript method");
	else
		return data_[index];
}

template <typename T>
T Base_array<T>::get (size_t index) const
{
	if (index >= cur_size_)
		throw std::out_of_range("\aOut of range error: get method");

	return data_[index];
}

template <typename T>
void Base_array<T>::set (size_t index, T value)
{
	if (index >= cur_size_)
		throw std::out_of_range("\aOut of range error: set method");

	data_[index] = value;
}

template <typename T>
int Base_array<T>::find (T value) const
{
	for (int i = 0; i < cur_size_; i++) {
		if (data_[i] == value)
	  	//Return the indexed value
			return i;
	}
	return -1;
}

template <typename T>
int Base_array<T>::find (T val, size_t start) const
{
	if (start >= cur_size_)
		throw std::out_of_range("\aOut of range error: find method");

	for (int i = start; i < cur_size_; i++)
	{
		if (data_[i] == val)
			//Return the indexed value
			return i;
	}
	return -1;
}

template <typename T>
bool Base_array<T>::operator == (const Base_array & rhs) const
{
	if(this == &rhs)
		return true;

	if (cur_size_ == rhs.cur_size_)
	{
		for (int i = 0; i < cur_size_; i++)
		{
			if (data_[i] != rhs.data_[i])
				return false;
		}
		return true;
	}
	else
		return false;
}

template <typename T>
bool Base_array<T>::operator != (const Base_array & rhs) const
{
	if(this == &rhs)
		return false;

	if (cur_size_ == rhs.cur_size_)
	{
		for (int i = 0; i < cur_size_; i++)
		{
			if (data_[i] != rhs.data_[i])
				return true;
		}
		return false;
	}
	else
		return true;
}

template <typename T>
void Base_array<T>::fill (T value)
{
	for (int i = 0; i < cur_size_; i++)
		data_[i] = value;
}

template <typename T>
void Base_array<T>::reverse (void)
{
	T temp;

	//Transfer the last element of data to the first element of data
	for (int i = 0; i < (cur_size_)/2; i++)
	{
		temp = data_[(cur_size_- 1) - i];
		data_[(cur_size_- 1) - i] = data_[i];
		data_[i] = temp;
	}
}
