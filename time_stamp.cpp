
#include "time_stamp.h"

void time_stamp(void)
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
	s -= 18000;

	//max days in the month
	uint16_t max_days_feb, max_days_mar, max_days_apr, max_days_may, max_days_jun;
	uint16_t max_days_jul, max_days_aug, max_days_sep, max_days_oct, max_days_nov;
	uint16_t max_days_dec;

	//subtraction values for formatting the days
	uint16_t subval_mar, subval_apr, subval_may, subval_jun, subval_jul, subval_aug;
	uint16_t subval_sep, subval_oct, subval_nov, subval_dec;

	//using modulus of numbers to calculate seconds, minutes, hours, etc.
	uint64_t seconds = s % 60;
	uint64_t minutes = s / 60 % 60;
	uint64_t days = (s / 86400);

	//(s/86400) is day calculation, multiply it by 24, after subtracting total seconds
	uint64_t hours = (s / 3600) - ((s / 86400) * 24);

	//gives months since 1970
	uint64_t months = s / 2629743;

	//this is the number of years since 1970
	uint64_t years = s / 31556926;

	//this is the actual year now
	uint64_t formated_years = years + 1970;

	//I take # of months since 1970, subtract it from a known value,
	//which will be the month of january for each year, and add 1 to that
	//to get the month for this year since it'll be from 0 to 11 instead
	//of 1-12
	uint64_t formated_months = (months - ((formated_years - 1970) * 12)) + 1;

	//years since 1970
	//for 2017 I get a day count of 17155 days, subtract from days calculated
	//from seconds, I get 54 as of now (2/2/2017).
	//There was 12 leap years since 1970, so I subtract that
	//and get 42, add one again for 1-31 for january, then we get february days
	//continue that for each month
	uint64_t formated_days = days - ((formated_years - 1970) * 365);
	//there's been 12 leap years since 1970,
	//subtract those extra days for this calculation
	formated_days -= 12;
	//add one for formatting purposes (its 0-42 now)
	formated_days += 1;


	//january is default, just use the original number
	//february(leap year, if divisible by 4)
	if (formated_years % 4 == 0)
	{
		if (formated_months == 2)
		{
			max_days_feb = 60;
			max_days_mar = 90;  subval_mar = 60;
			max_days_apr = 121; subval_apr = 90;
			max_days_may = 151; subval_may = 121;
			max_days_jun = 182; subval_jun = 151;
			max_days_jul = 213; subval_jul = 182;
			max_days_aug = 243; subval_aug = 213;
			max_days_sep = 274; subval_sep = 243;
			max_days_oct = 304; subval_oct = 274;
			max_days_nov = 335; subval_nov = 304;
			max_days_dec = 366; subval_dec = 335;
		}
	}
	else{
		max_days_feb = 59;
		max_days_mar = 89;  subval_mar = 59;
		max_days_apr = 120; subval_apr = 89;
		max_days_may = 150; subval_may = 120;
		max_days_jun = 181; subval_jun = 150;
		max_days_jul = 212; subval_jul = 181;
		max_days_aug = 242; subval_aug = 212;
		max_days_sep = 273; subval_sep = 242;
		max_days_oct = 303; subval_oct = 273;
		max_days_nov = 334; subval_nov = 303;
		max_days_dec = 365; subval_dec = 334;
	}


	//february
	if (formated_days > 31 && formated_days <= max_days_feb) formated_days -= 31;
	//march
	if (formated_days > max_days_feb && formated_days <= max_days_mar) formated_days -= subval_mar;
	//april
	if (formated_days > max_days_mar && formated_days <= max_days_apr) formated_days -= subval_apr;
	//may
	if (formated_days > max_days_apr && formated_days <= max_days_may) formated_days -= subval_may;
	//june
	if (formated_days > max_days_may && formated_days <= max_days_jun) formated_days -= subval_jun;
	//july
	if (formated_days > max_days_jun && formated_days <= max_days_jul) formated_days -= subval_jul;
	//august
	if (formated_days > max_days_jul && formated_days <= max_days_aug) formated_days -= subval_aug;
	//september
	if (formated_days > max_days_aug && formated_days <= max_days_sep) formated_days -= subval_sep;
	//october
	if (formated_days > max_days_sep && formated_days <= max_days_oct) formated_days -= subval_oct;
	//november
	if (formated_days > max_days_oct && formated_days <= max_days_nov) formated_days -= subval_nov;
	//december
	if (formated_days > max_days_nov && formated_days <= max_days_dec) formated_days -= subval_dec;


	/* Formatted Local Time (Indianapolis) */
	std::cout << formated_years << "/" << formated_months << "/" << formated_days;
	std::cout << " " << hours << ":" << minutes << ":" << seconds << "." << ns;
}
