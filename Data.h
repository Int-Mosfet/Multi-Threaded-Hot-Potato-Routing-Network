#ifndef _DAT_H
#define _DAT_H

#include <ctime>

struct Data
{
    unsigned int rx;
    unsigned int tx;
    unsigned int src;
    unsigned int dst;
    unsigned long time_to_live;
    unsigned int hop_count;
    unsigned int type;
    unsigned int rsrvd;
    time_t tm;
};

#endif
