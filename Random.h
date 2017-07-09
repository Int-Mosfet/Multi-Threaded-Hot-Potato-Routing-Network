#ifndef _RAN_H
#define _RAN_H
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <random>

#define NANO_SCALE 1000000000

double random_uniform();
double random_exponential(double mean);
double random_normal(double mean, double dev);
int random_value(int range);
unsigned int rand_select(void);

#endif
