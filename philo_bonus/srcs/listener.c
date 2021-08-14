#include "philo_bonus.h"

/*
** wait for the end of simulation's signal 
** then kill the philosopher and propagates signal
** to others philos
*/
static void *end_simu_listener(void *philosopher)
{
	t_philo *philo;

	philo = (t_philo *)philosopher;
	log_philo("listener started", philo);
	if ( sem_wait(philo->simu->stop_simu) != SUCCESS)
	{
		printf("%ld %3d   critical system error in semaphore (stop_simu)\n", elapsedStart(philo->timestamp), philo->id);	
		return (NULL);
	}
	log_philo("signal 'end of simulation' received", philo);
	philo->alive = FALSE;
	if (sem_post(philo->simu->stop_simu) != SUCCESS)
	{
		printf("%ld %3d   critical system error in semaphore (stop_simu)\n", elapsedStart(philo->timestamp), philo->id);	
		return (NULL);
	}
	return (NULL);
}

/*
** start a listener that wait for the end of the simulation
*/
int	start_end_simu_listener(t_philo *philo)
{
	int			err;
	pthread_t	listener_id;

	if ((err = pthread_create(&listener_id, NULL, &end_simu_listener, (void *)philo)) != 0)
	{
	   printf("\ncan't create listener thread :[%d]", err);
		return (SYS_ERROR);
	}
	if (pthread_detach(listener_id) != SUCCESS)
		return (SYS_ERROR);
	return (SUCCESS);
}

/*
** wait that all philosophers have eaten enough
*/
static void *eat_enough_listener(void *simulation)
{
	t_philo_simu *simu;

	simu = (void *)simulation;
	log_simu("eat enough listener started.", simu);
	while (simu->count_philo_eat_enough < simu->number_of_philosopher)
	{
		if (sem_wait(simu->eat_enough) != SUCCESS)
		{
			printf("%ld   -   critical system error in semaphore (eat_enough)\n", elapsedStart(simu->timestamp));	
			return (NULL);
		}
		simu->count_philo_eat_enough ++;
	}
	log_simu("all the philosophers have eaten enough.", simu);
	if (sem_post(simu->stop_simu) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (stop_simu)\n", elapsedStart(simu->timestamp));	
		return (NULL);
	}
	return (NULL);
}


/*
** start a listener waiting for all the philosophers to have eaten enough
*/
int	start_eat_enough_listener(t_philo_simu *simu)
{
	int			err;
	pthread_t	listener_id;

	if ((err = pthread_create(&listener_id, NULL, &eat_enough_listener, (void *)simu)) != 0)
	{
	   printf("\ncan't create listener thread :[%d]", err);
		return (SYS_ERROR);
	}
	if (pthread_detach(listener_id) != SUCCESS)
		return (SYS_ERROR);
	return (SUCCESS);
}
