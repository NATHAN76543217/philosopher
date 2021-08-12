#include "philo_bon.h"

/*
** wait for the signal of the end of simulation
*/
static void *end_simu_listener(void *philosopher)
{
	t_philo *philo;

	philo = (t_philo *)philosopher;
	sem_wait(philo->simu->stop_simu);
	log_philo("signal 'end of simulation' received", philo);
	philo->alive = FALSE;
	sem_post(philo->simu->stop_simu);
	return NULL;
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
	while (simu->count_philo_eat_enough < simu->number_of_philosopher)
	{
		sem_wait(simu->eat_enough);
		simu->count_philo_eat_enough ++;
	}
	log_simu("all the philosophers have eaten enough.", simu);
	sem_post(simu->stop_simu);
	return NULL;
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
