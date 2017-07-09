#include "Current_Time.h"

Current_Time::Current_Time()
:curr_time(0)
,prev_time(0)
,time_diff(0)
,time_diff_sqr(0)
,mean(0)
,dev(0)
,s(0)
,xi(0)
,variance(0)
{
	clock_gettime(CLOCK_REALTIME, &ts);
	prev_time = ts.tv_sec;
	prev_time -= 21600;
}
time_t Current_Time::curr_time_sec()
{
	time_t s; //seconds
	long ns; //nanoseconds

	timespec ts;

	//getting the time (seconds since epoch)
	clock_gettime(CLOCK_REALTIME, &ts);
	//
	//getting the seconds and nanoseconds
	s = ts.tv_sec;
	ns = ts.tv_nsec;
	//to make it local time (from GMT), subtract 18,000 seconds if 5 hours behind
	//or 21600 if 6 hours behind
	s -= 21600;
	return s;
}

long Current_Time::curr_time_nsec()
{
	time_t s; //seconds
	long ns; //nanoseconds

	timespec ts;

	//getting the time (seconds since epoch)
	clock_gettime(CLOCK_REALTIME, &ts);

	//getting the seconds and nanoseconds
	s = ts.tv_sec;
	ns = ts.tv_nsec;
	//to make it local time (from GMT), subtract 18,000 seconds if 5 hours behind
	//or 21600 if 6 hours behind
	s -= 21600;
	return ns;
}

time_t Current_Time::get_time()
{
	curr_time = curr_time_sec();
	time_diff += curr_time - prev_time;
	prev_time = curr_time;
	time_diff_sqr += time_diff * time_diff;
	return time_diff;
}
