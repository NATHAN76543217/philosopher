#include "philo_bon.h"

/*
** The log_philo() function print a philosopher status well formated 
*/
void		log_philo(char *str, const t_philo *philo)
{
	if ( sem_wait(philo->simu->writing) != SUCCESS)
	{	
		printf("%ld %3d   critical system error in semaphore (writing)\n", elapsedStart(philo->timestamp), philo->id);	
		return ;
	}
	printf("%ld %3d   %s\n", elapsedStart(philo->timestamp), philo->id, str);	
	if (sem_post(philo->simu->writing) != SUCCESS)
	{
		printf("%ld %3d   critical system error in semaphore (writing)\n", elapsedStart(philo->timestamp), philo->id);	
		return ;
	}
}

/*
** The log_simu() function print simulation status well formated 
*/
void		log_simu(char *str, const t_philo_simu *simu)
{
	if ( sem_wait(simu->writing) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (writing)\n", elapsedStart(simu->timestamp));	
		return ;
	}
	printf("%ld   -   %s\n", elapsedStart(simu->timestamp), str);	
	if (sem_post(simu->writing) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (writing)\n", elapsedStart(simu->timestamp));	
		return ;
	}
}

/*
** The error_msg() function print a given error before returning a given value 
*/
int			error_msg(const char *const str, const int ret_value)
{
	printf("%s", str);
	return ret_value;
}
