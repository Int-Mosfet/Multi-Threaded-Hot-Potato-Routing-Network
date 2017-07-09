#ifndef _ARGC_H
#define _ARGC_H

#include <sstream>

//Converter
template <typename type>
type convert(char* arg)
{
	std::istringstream iss(arg);
	type argv_value;
	//convert the value
	iss >> argv_value;
	return argv_value;
}

#endif
