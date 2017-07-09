#define HEAD 0

template <typename T>
Queue <T>::Queue (size_t size)
  :top(HEAD)
{
  array.resize(size);
  array.shrink();
  array.resize(HEAD);
}

//
// Queue
//
template <typename T>
Queue <T>::Queue (const Queue & queue)
  :top(HEAD)
{
  for(int i = 0; i < queue.array.size(); i++)
    this->enqueue(queue.array.get(i));
}

//
// enqueue
//
template <typename T>
void Queue <T>::enqueue (T element)
{
  /*save the Current size, then resize
    to max to allow the 'set' function
    to be implemented due to "index >= cur_size_"
  */
  size_t temp;

  temp = this->array.size();
  if(this->array.size() >= this->array.max_size())
	std::cout<<"Mailbox is full"<<std::endl;
  else{
    this->array.resize(temp+1);
    this->array.set(temp,element);
  }
}

//
// dequeue
//
template <typename T>
T Queue <T>::dequeue (void)
{
  if(is_empty())
    throw std::domain_error("Error! \a-> Queue is empty: dequeue method");

  T temp = this->array.get(top);
  size_t index;

  for(index = HEAD; index < this->array.size()-1; index++)
    this->array.set(index,this->array.get(index+1));

  this->array.resize(this->array.size()-1);
  return temp;
}

//
// operator =
//

template <typename T>
const Queue <T> & Queue <T>::operator = (const Queue & rhs)
{
  //Checking for self assignment
  if(this == &rhs)
    return *this;

  size_t temp_size;

  //Creating a copy of rhs queue
  //to modify values for operator= algorithm
  Queue<T> temp_queue(rhs);
  this->array.resize(temp_queue.size());

  if(this->max_size() > this->size())
    this->array.shrink();

  std::cout<<std::endl;
  temp_size = this->size();
  temp_queue.array.resize(temp_queue.max_size());
  this->array.resize(HEAD);

  for(int i = 0; i < temp_size; i++)
    this->enqueue(temp_queue.array.get(i));
}

//
// clear
//

template <typename T>
void Queue <T>::clear(void)
{
  this->array.resize(HEAD);
}

template <typename T>
void Queue<T>::resize(size_t cap_of_buf)
{
  this->array.resize(cap_of_buf);
  this->array.shrink();
  this->array.resize(HEAD);
}
