#include "philo_bon.h"

/*
** The log_philo() function print a philosopher status well formated 
*/
void		log_philo(char *str, const t_philo *philo)
{
	printf("%ld %3d  %s\n", elapsedStart(philo->timestamp), philo->id, str);	
}

/*
** The log_simu() function print simulation status well formated 
*/
void		log_simu(char *str, const t_philo_simu *simu)
{
	printf("%ld simu %s\n", elapsedStart(simu->timestamp), str);	
}

/*
** The error_msg() function print a given error before returning a given value 
*/
int			error_msg(const char *const str, const int ret_value)
{
	printf("%s", str);
	return ret_value;
}
