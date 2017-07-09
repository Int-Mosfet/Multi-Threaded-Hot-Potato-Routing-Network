#ifndef _CS507_QUEUE_H_
#define _CS507_QUEUE_H_

#include <exception>
#include "Array.h"

/**
 * @class Queue
 *
 * Basic queue for abitrary elements.
 */
template <typename T>
class Queue
{
public:
  /// Type definition of the type.
  typedef T type;

  /// Default constructor.
  Queue (size_t size);

  /// Copy constructor.
  Queue (const Queue & q);

  /**
   * Assignment operator
   *
   * @param[in]      rhs           Right-hand side of operator
   * @return         Reference to self
   */
  const Queue & operator = (const Queue & rhs);

  /**
   * Adds a new element to the end of the list.
   *
   * @param[in]      element       Element to add to the list
   */
  void enqueue (type element);

  /**
   * Removes the element at the front of the list.
   * If there are no elements in the queue, throw empty_exception.
   *
   * @exception      empty_exception    The queue is empty.
   */
  T dequeue (void);

  /**
   * Test if the stack is empty
   *
   * @retval         true          The queue is empty
   * @retval         false         The queue is not empty
   */

  bool is_empty (void) const;

  bool is_full(void) const;

  /**
   * Number of element in the queue.
   *
   * @return         Size of the queue.
   */
  size_t size (void) const;

  /**
   * Retrieve the maximum size of the queue.
   *
   * @return          The maximum size
   */
  size_t max_size (void) const;

  /// Remove all elements from the queue.
  void clear (void);

  void resize(size_t);

private:
  Array<T> array;
  size_t top;
};

// include the inline files
#include "Queue.inl"

// include the source file since template class
#include "Queue.cpp"

#endif  // !defined _CS507_QUEUE_H_
