#include "Array.h"
#include<iostream>
#include<stdexcept>

//default constructor
template<typename T>
Array<T>::Array (void)
	:Base_array<T>()
{

}

template<typename T>
Array<T>::Array (size_t length)
	:Base_array<T>(length)
{

}

template<typename T>
Array<T>::Array (size_t length, T fill)
	:Base_array<T>(length,fill)
{

}

template <typename T>
Array<T>::Array (const Array & array)
	:Base_array<T>(array)
{

}

template <typename T>
const Array<T> & Array<T>::operator = (const Array & rhs)
{
	//Checking for self assignment
	if(this == &rhs)
		return *this;

	T* data_;

	//Retrieve the memory location
	data_ = this->get_data();

	//If the max sizes are not equal then make lhs == rhs
	if(this->max_size() != rhs.max_size())
	{
		delete[] data_;
		data_ = new T[rhs.max_size()];
		this->set_size(rhs.size());
		this->set_max(rhs.max_size());
		this->set_data(data_);
	}
	for (int i = 0; i < this->max_size(); i++)
		this->set(i,rhs.get(i));
	return *this;
}

template <typename T>
T & Array<T>::operator [] (size_t index)
{
	T* data_;

	data_ = this->get_data();
	if (index >= this->size())
		throw std::out_of_range("\aOut of range error: subscript method");
	else
		return data_[index];
}

template <typename T>
const T & Array<T>::operator [] (size_t index) const
{
	T* data_;

	data_ = this->get_data();

	if (index >= this->size())
		throw std::out_of_range("\aOut of range error: subscript method");
	else
		return data_[index];
}

template <typename T>
void Array<T>::resize (size_t new_size)
{
	T* temp;
	T* data_;

	data_ = this->get_data();

	if (new_size > this->max_size())
	{
		//Allocating memory for temp
		temp = new T[new_size];

		//Transfering the data
		for (int i = 0; i < this->max_size();i++)
			temp[i] = data_[i];

		//Assigning the new location to data_
		delete[] data_;
		this->set_data(temp);
		this->set_size(new_size);
		this->set_max(new_size);
		temp = NULL;
	}
	this->set_size(new_size);
}

template <typename T>
bool Array<T>::operator == (const Array & rhs) const
{
	if (this == &rhs)
		return true;

	if (this->size() == rhs.size())
	{
		for (int i = 0; i < this->size(); i++)
		{
			if (this->get(i) != rhs.get(i))
				return false;
		}
		return true;
	}
	else
		return false;
}

template <typename T>
bool Array<T>::operator != (const Array & rhs) const
{
	if(this == &rhs)
		return false;

	if (this->size() == rhs.size())
	{
		for (int i = 0; i < this->size(); i++)
		{
			if (this->get(i) != rhs.get(i))
				return true;
		}
		return false;
	}
	else
		return true;
}

template <typename T>
void Array<T>::shrink (void)
{
	if(this->size() < this->max_size())
	{
	  T* temp;

		temp = new T[this->size()];

		for (int i = 0; i < this->size(); i++)
			temp[i] = this->get(i);

		T* data_;
		data_ = this->get_data();
		delete[] data_;
		data_ = temp;
		this->set_data(data_);
		temp = NULL;
		this->set_max(this->size());
	}
}
