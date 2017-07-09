#include "Neighbor.h"

Neighbor::Neighbor()
  :index(0)
{
  /*We will be using the default array
   constructor because we do not know
   how many nodes there are*/
}

void Neighbor::set_neighbor(unsigned int num)
{
  //Put the nodes Neighbor in the array.
  this->arr.resize(this->arr.size()+1);
  this->arr.set(this->arr.size()-1,num);
}

unsigned int Neighbor::get_neighbor(size_t i)
{
  /*Return neighbor at index i*/
  return this->arr.get(i);
}

size_t Neighbor::size()
{
  return this->arr.size();
}
