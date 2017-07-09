#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include "Queue.h"
#include <iostream>
#include <stdbool.h>
#include "Random.h"
#include "Sleep.h"
#include "time_stamp.h"
#include "Mailbox.h"
#include "Current_Time.h"
#include "Array.h"
#include "Data.h"
#include "Current_Time.h"
#include "Neighbor.h"
#include <fstream>

extern int num_nodes;//added
#define DEFAULT_TTL 5

class Thread
{
public:
  Thread(Array<Mailbox*>&,Array<Neighbor*>&,std::ofstream&,pthread_mutex_t&);
  int create_node(Array<pthread_t>&,pthread_t,bool);
  void set_sleep(double con, double pro);
  void set_id(pthread_t);
  void set_outputflag(bool);
  int neighbor_id(int);
  time_t begin = tme_.curr_time_sec();//added
  bool ping_flag = false;//added
  int random_neighbor;//added

 private:
  static void* producer(void*);
  static void* consumer(void*);
  void prod(pthread_t);
  void cons(pthread_t);
  void reply(Data*);
  void relay_to_neighbor(pthread_t,Message*);
  pthread_mutex_t& the_mutex;
  pthread_cond_t c_cons;
  pthread_cond_t p_prod;
  bool KeepGoing;
  bool output_flag;
  double con_mean_sleep_time;   //(Alpha) consumer's mean sleep time
  double prod_mean_sleep_time;  //(Beta)  producer's mean sleep time
  pthread_t nodeID;
  unsigned int frequency;
  struct timespec current_time;
  Array<Mailbox*>& mailbox;
  Array<Neighbor*>& my_neighbor;
  Current_Time tme_;
  std::ofstream& outfile;
  int msg_drops;
  Data info;
};

#endif
