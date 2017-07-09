//
// size
//
template <typename T>
inline
size_t Queue <T>::size (void) const
{
  return this->array.size();
}

template <typename T>
inline
size_t Queue<T>::max_size(void) const
{
  return this->array.max_size();
}

//
// is_empty
//
template <typename T>
inline
bool Queue <T>::is_empty (void) const
{
  if(this->array.size() == top)
    return true;

  return false;
}

template <typename T>
inline
bool Queue <T>::is_full (void) const
{
  if(this->array.size() == this->array.max_size())
    return true;

  return false;
}
