#ifndef _MAIL_H_
#define _MAIL_H_

#include "Queue.h"
#include "Message.h"
#include "Data.h"

class Mailbox
{
public:
  Mailbox(size_t size);
  ~Mailbox();
  double put(Message*);
  Message* get(void);
  int count(void);
  int maximum(void);
  bool isFull(void);
  bool isEmpty(void);
private:
  Queue<Message*> queue;
};
#endif
