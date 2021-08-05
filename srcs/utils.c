#include "philo.h"

/*
** Return the number of milliseconds elapsed since the start of simulation
*/

long elapsedStart(t_philo *philo)
{
	struct timeval tInit;

	gettimeofday(&tInit, NULL);
	long sec_ini = (long) philo->timestamp->tv_sec; 
	long milli_ini = (long) philo->timestamp->tv_usec;
	long sec = (long) tInit.tv_sec; 
	long milli = (long) tInit.tv_usec;
	return ((sec - sec_ini) * 1000000 + (milli - milli_ini)) / 1000;
}

/*
** Return the number of milliseconds elapsed since the last meal
*/

long elapsedLastMeal(t_philo *philo)
{
	struct timeval tInit;

	gettimeofday(&tInit, NULL);
	long sec_ini = (long) philo->last_meal->tv_sec; 
	long milli_ini = (long) philo->last_meal->tv_usec;
	long sec = (long) tInit.tv_sec; 
	long milli = (long) tInit.tv_usec;
	long micro_s = ((sec - sec_ini) * 1000000 + (milli - milli_ini));
	#ifdef DEBUG
		printf("%ld %d since last meal: %ld,%ld.\n", elapsedStart(philo), philo->id, micro_s / 1000, micro_s % 1000);
	#endif
	return  micro_s / 1000;
}

/*
** The isdigit() function tests for a decimal digit character.
*/

static int ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

/*
** The ft_atoi() function converts the initial portion of the string pointed
** to by str to int representation.
*/

int		ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (*(str + i) == ' ' || (*(str + i) >= '\t' && *(str + i) <= '\r'))
		i++;
	if (*(str + i) == '+' || *(str + i) == '-')
		(*(str + i++) == '-') ? sign = -1 : 0;
	while (ft_isdigit(*(str + i)))
	{
		if (result * sign > INT32_MAX)
			return (-1);
		else if (result * sign < INT32_MIN)
			return (0);
		result = result * 10 + (*(str + i++) - '0');
	}
	result *= sign;
	return (result);
}

/*
** The error_msg() function print a given error before returning a given value 
*/

int		error_msg(const char *str, int ret_value)
{
	printf("%s", str);
	return ret_value;
}