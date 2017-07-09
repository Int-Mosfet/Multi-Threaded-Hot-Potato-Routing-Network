#ifndef _NEIGH_H
#define _NEIGH_H

#include "Array.h"

class Neighbor
{
public:
  Neighbor();
  void set_neighbor(unsigned int);
  unsigned int  get_neighbor(size_t);
  size_t size();
private:
  unsigned int num_of_neighbors;
  Array <int> arr;
  int index;
};

#endif
