#include "Sleep.h"
#include <stdlib.h>

int nsleep(long nseconds)
{
	struct timespec req;
	int rval;

	req.tv_sec = (time_t)nseconds/1e+9;
	req.tv_nsec = (long)(nseconds - req.tv_sec*1e+9);
	//If rval is then finished full sleep
	//else it will return -1.
	do
	{
		rval = nanosleep(&req,&req);
		if(rval == -1 && errno == EINTR)
		{
			std::cout<<"Time remaining: "<<req.tv_sec<<"."<<req.tv_nsec<<std::endl;
		}
		else if(rval == -1 && errno == EINVAL)
		{
			std::cout<<"Error EINVAL"<<std::endl;
			exit(0);
		}
		else if(rval == -1 && errno == EFAULT)
		{
			std::cout<<"Error EFAULT"<<std::endl;
			exit(0);
		}
	}while(rval != 0);
	return  rval;
}
