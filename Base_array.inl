template <typename T>
inline
size_t Base_array<T>::size (void) const
{
  return cur_size_;
}

template <typename T>
inline
size_t Base_array<T>::max_size (void) const
{
  return max_size_;
}

template <typename T>
inline
void Base_array<T>::set_size(size_t new_size)
{
  cur_size_ = new_size;
}

template <typename T>
inline
void Base_array<T>::set_max(size_t new_size)
{
  max_size_ = new_size;
}

template <typename T>
inline
T* Base_array<T>::get_data()
{
  return data_;
}

template <typename T>
inline
void Base_array<T>::set_data(T* new_data)
{
  data_ = new_data;
}
