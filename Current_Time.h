#ifndef _CURTIME_H
#define _CURTIME_H

#include <ctime>

class Current_Time
{
public:
  Current_Time(void);
  time_t curr_time_sec();
  long curr_time_nsec();
  time_t get_time(void);
  time_t curr_time = 0;
  time_t prev_time = 0;
  time_t time_diff = 0;
  time_t time_diff_sqr = 0;
  time_t mean = 0;
  time_t dev = 0;
  time_t s = 0;
  time_t xi = 0;
  time_t variance = 0;
private:
  timespec ts;
};

#endif
