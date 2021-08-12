#include "philo_bon.h"

/*
** Philosopher lifecycle
*/
static void	*routine(t_philo* philo)
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
		sem_post(philo->simu->forks);
		sem_post(philo->simu->forks);
	}
	return (philo);
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
	routine(philo);
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
	// philo.forks_in_hand = 0;
	init_timestamps(&philo, simu->timestamp);
	init_activities(&philo);	
	if ((simu->philos_id[id] = fork()) == 0)
		philosopher(&philo);
	else if (simu->philos_id[id] < 0)
		return error_msg("Error while forking processes.", SYS_ERROR);
	return SUCCESS;
}

/*
** init and launch a philosopher thread
*/
// int     	create_philosopher(t_philo_simu *simu, int id)
// {
	// int				err;
	// t_philo*		philo;
// 
	// if ((philo = (t_philo *) malloc(sizeof(t_philo))) == NULL)
		// return error_msg("A memory error happen when malloc\n", MEMORY_ERROR);
	// philo->id = id + 1;
	// philo->simu = simu;
	// philo->eat_count = 0;
	// philo->alive = TRUE;
	// philo->left_fork_id = ( id % 2) ? id : (id + 1) % simu->number_of_philosopher;
	// philo->right_fork_id = ( id % 2) ? (id + 1) % simu->number_of_philosopher : id;
	// init_activities(philo);
	// if ( simu->number_of_philosopher == 1 )
		// philo->left_fork_id = 1;
	// if ((err = init_timestamps(philo, simu->timestamp)) != SUCCESS)
		// return err;
	// if (pthread_mutex_init(&(philo->philo_m), NULL) != SUCCESS)
		// return error_msg("mutex initialisation for philosopher failed\n", SYS_ERROR);
	// simu->philos[id] = philo;
	// if ((err = pthread_create(&(simu->threads[id]), NULL, &routine, (void *)philo)) != 0)
		//    printf("\ncan't create thread :[%d]", err);
	// return SUCCESS;
// }