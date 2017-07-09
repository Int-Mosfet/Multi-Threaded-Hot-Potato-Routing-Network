#include "Random.h"

double random_uniform()
{
	sleep(1);
	//Seeding
	srand((unsigned int)time(NULL));
	double mail;

	mail = ((double)rand()/RAND_MAX);
	return mail;
}

double random_exponential(double mean)
{
	//returns in nanoseconds
	return (-mean*log(1 - random_uniform())*NANO_SCALE);
}

unsigned int rand_select(void)
{
	int weight = rand() % 7;
	unsigned int rx;
	switch( weight )
	{
		case 0:
			rx = 0;
			break;
		case 1:
			rx = 1;
			break;
		case 2:
			rx = 2;
			break;
		case 3:
			rx = 3;
			break;
		case 4:
			rx = 4;
			break;
		case 5:
			rx = 5;
			break;
		case 6:
			rx = 6;
			break;
	}
	if(rx == 6)
		rx = 0xFFFFFFFF;
	return rx;
}

double random_normal(double mean, double dev)
{
	/*Return value: a random number following a normal distribution
	  whose average and standard deviation are approximated by the
	  arguments*/

	//seed the random number engine class
	std::random_device rd;
	std::default_random_engine generator(rd());

	double normal = 0.0;

	//generate the normal distribution with specified mean and standard deviation
	std::normal_distribution<double> distribution(mean, dev);
	normal = distribution(generator);

	return normal;
}

int random_value(int range)
{
	return rand()%range;
}
