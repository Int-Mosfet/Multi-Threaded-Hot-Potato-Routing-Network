#include "Mailbox.h"

Mailbox::Mailbox(size_t size)
  :queue(size)
{

}

Mailbox::~Mailbox()
{
  //Does nothing for right now
  //maybe we need to free the memory
  //from the struct Message.
}

double Mailbox::put(Message* msg)
{
  queue.enqueue(msg);
  return *((double*)msg->data);
}

Message* Mailbox::get()
{
  return queue.dequeue();
}

int Mailbox::count()
{
  return queue.size();
}

int Mailbox::maximum()
{
  return queue.max_size();
}

bool Mailbox::isFull()
{
  return queue.is_full();
}

bool Mailbox::isEmpty()
{
  return queue.is_empty();
}
