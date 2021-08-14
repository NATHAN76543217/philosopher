#include "philo_bonus.h"

/*
** Philosopher lifecycle
*/
static int	routine(t_philo* philo)
{
	int			i;
	int			ret;

	i = 0;
	while(philo->alive)
	{
		if ((ret = philo->activity[i](philo)) != SUCCESS)
		{
			if (i == THINKING && ret == EXIT_FAILURE)
				break ;
			philo->alive = FALSE;
		}
		i = (i + 1) % 3;
	}
	if (i == EATING)
	{
		if (sem_post(philo->simu->forks) != SUCCESS)
		{
			printf("%ld %3d   critical system error in semaphore (forks)\n", elapsedStart(philo->timestamp), philo->id);	
			return (SYS_ERROR);
		}
		if (sem_post(philo->simu->forks) != SUCCESS)
		{
			printf("%ld %3d   critical system error in semaphore (forks)\n", elapsedStart(philo->timestamp), philo->id);	
			return (SYS_ERROR);
		}
		log_philo("has release two forks.", philo);
	}
	return (SUCCESS);
}

/*
** philosophers process entrypoint
*/
static void *philosopher(void *philosopher)
{
	t_philo*	philo;

	philo = (t_philo *) philosopher;
	log_philo("is starting", philo);
	start_monitoring(philo);
	exit(routine(philo));
	return (NULL);
}


/*
** Initialise all timestamp values in a philosopher
*/
static int	init_timestamps(t_philo *philo, struct timeval start_timestamp)
{
	ft_memcpy((void *) &(philo->timestamp), (void *) &start_timestamp, sizeof(struct timeval));
	ft_memcpy((void *) &(philo->last_meal), (void *) &start_timestamp, sizeof(struct timeval));
	return SUCCESS;
}



/*
** init the array of pointers on function
*/
static void	init_activities(t_philo *philo)
{
	philo->activity[THINKING] = &take_forks;
	philo->activity[EATING] = &philo_eat;
	philo->activity[SLEEPING] = &philo_sleep;
}


/*
** init and launch a philosopher process
*/
int     	create_philosopher(t_philo_simu *simu, int id)
{
	t_philo		philo;

	philo.id = id + 1;
	philo.simu = simu;
	philo.eat_count = 0;
	philo.alive = TRUE;
	// philo.forks_in_hand = 0;
	init_timestamps(&philo, simu->timestamp);
	init_activities(&philo);	
	if ((simu->philos_id[id] = fork()) == 0)
		philosopher(&philo);
	else if (simu->philos_id[id] < 0)
		return error_msg("Error while forking processes.", SYS_ERROR);
	return SUCCESS;
}
