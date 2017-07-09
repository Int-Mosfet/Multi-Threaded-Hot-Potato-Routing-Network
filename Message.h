#ifndef _MSG_H
#define _MSG_H

struct Message
{
  //The size of the type of data we have
  int bytes;
  //This will hold the message
  void* data;
  //This will point to the new struct
  void* data2;

};

#endif
