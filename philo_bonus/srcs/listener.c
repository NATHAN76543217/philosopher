#include "philo_bonus.h"

/*
** wait for the end of simulation's signal 
** then kill the philosopher and propagates signal
** to others philos
*/
static void	*end_simu_listener(void *philosopher)
{
	t_philo *philo;

	philo = (t_philo *)philosopher;
	log_philo("listener started", philo);
	if ( sem_wait(philo->simu->stop_simu) != SUCCESS)
	{
		log_syserror("critical system error in semaphore (stop_simu)", philo);
		return (NULL);
	}
	log_philo("signal 'end of simulation' received", philo);
	philo->alive = FALSE;
	if (sem_post(philo->simu->stop_simu) != SUCCESS)
	{
		log_syserror("critical system error in semaphore (stop_simu)", philo);
		return (NULL);
	}
	log_philo("end_simu listener stopped.", philo);
	return (NULL);
}

/*
** start a listener that wait for the end of the simulation
*/
int				start_end_simu_listener(t_philo *philo)
{
	int			err;
	pthread_t	listener_id;

	if ((err = pthread_create(&listener_id, NULL, &end_simu_listener, (void *)philo)) != 0)
		return (log_syserror("can't create end_simu listener", philo));
	if (pthread_detach(listener_id) != SUCCESS)
		return (log_syserror("can't detach end_simu listener", philo));
	return (SUCCESS);
}

/*
** wait that all philosophers have eaten enough
*/
static void	*eat_enough_listener(void *simulation)
{
	t_philo_simu *simu;

	simu = (void *)simulation;
	log_simu("eat enough listener started.", simu);
	while (simu->count_philo_eat_enough < simu->number_of_philosopher && simu->running == TRUE)
	{
		if (sem_wait(simu->eat_enough) != SUCCESS)
		{
			printf("%ld   -   critical system error in semaphore (eat_enough)\n", elapsedStart(simu->timestamp));	
			return (NULL);
		}
		simu->count_philo_eat_enough ++;
	}
	if (simu->running == FALSE)
	{
		log_simu("eat_enough listener stopped.", simu);
		return (NULL);
	}	
	log_simu("all the philosophers have eaten enough.", simu);
	if (sem_post(simu->stop_simu) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (stop_simu)\n", elapsedStart(simu->timestamp));	
		return (NULL);
	}
	log_simu("eat_enough listener stopped.", simu);
	return (NULL);
}

/*
** stop and join eat_enough listener
*/
int			stop_eat_enough_listener(t_philo_simu *simu)
{
	if (sem_post(simu->eat_enough) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (eat_enought)\n", elapsedStart(simu->timestamp));	
		return (SYS_ERROR);
	}
	if (pthread_join(simu->listener_id, NULL) != SUCCESS)
	{
		log_simu("failed to join thtread eat_enough listener", simu);
		return (SYS_ERROR);
	}
	log_simu("eat_enough listener joined.", simu);
	return (SUCCESS);
}

/*
** start a listener waiting for all the philosophers to have eaten enough
*/
int			start_eat_enough_listener(t_philo_simu *simu)
{
	int			err;

	if ((err = pthread_create(&(simu->listener_id), NULL, &eat_enough_listener, (void *)simu)) != 0)
		return (error_msg("can't create eat_enough listener", SYS_ERROR));
	return (SUCCESS);
}
