#include "philo.h"

/*
** Warn other philosophers to stop the simulation
*/
static void	stop_simulation(const t_philo *philo)
{
	pthread_mutex_lock(&(philo->simu->simu_m));
	philo->simu->running = FALSE;
	log_philo("died.", philo);
	pthread_mutex_unlock(&(philo->simu->simu_m));
	log_philo("End of simulation.", philo);
}

/*
** Philosopher lifecycle
*/
static void	*routine(void* philosopher)
{
	int			i;
	int			ret;
	t_philo*	philo;

	i = 0;
	philo = (t_philo *) philosopher;
	log_philo("is starting", philo);
	while(!shouldStopSimu(philo))
	{
		if ((ret = philo->activity[i](philo)) != SUCCESS)
		{
			if (i == THINKING && ret == EXIT_FAILURE)
				break ;
			pthread_mutex_lock(&(philo->philo_m));
			philo->alive = FALSE;
			pthread_mutex_unlock(&(philo->philo_m));
		}
		i = (i + 1) % 3;
	}
	if (i == EATING)
	{
		pthread_mutex_unlock(&(philo->simu->forks[philo->left_fork_id]));
		pthread_mutex_unlock(&(philo->simu->forks[philo->right_fork_id]));
	}
	if (philo->simu->running)
		stop_simulation(philo);
	return philo;
}

/*
** Initialise all timestamp values in a philosopher
*/
static int	init_timestamps(t_philo *philo, struct timeval start_timestamp)
{
	struct timeval *timestamp[2];

	if (( timestamp[0] = (struct timeval*) malloc( sizeof( struct timeval ))) == NULL
	||	( timestamp[1] = (struct timeval*) malloc( sizeof( struct timeval ))) == NULL )
			return error_msg("A memory error happen when malloc (philo)\n", MEMORY_ERROR);
	philo->timestamp = ft_memcpy((void *) timestamp[0], (void *) &start_timestamp, sizeof(struct timeval));
	philo->last_meal = ft_memcpy((void *) timestamp[1], (void *) &start_timestamp, sizeof(struct timeval));
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
** init and launch a philosopher thread
*/
int     	create_philosopher(t_philo_simu *simu, int id)
{
	int				err;
	t_philo*		philo;

	if ((philo = (t_philo *) malloc(sizeof(t_philo))) == NULL)
		return error_msg("A memory error happen when malloc\n", MEMORY_ERROR);
	philo->id = id + 1;
	philo->simu = simu;
	philo->eat_count = 0;
	philo->alive = TRUE;
	philo->left_fork_id = ( id % 2) ? id : (id + 1) % simu->number_of_philosopher;
	philo->right_fork_id = ( id % 2) ? (id + 1) % simu->number_of_philosopher : id;
	init_activities(philo);
	if ( simu->number_of_philosopher == 1 )
		philo->left_fork_id = 1;
	if ((err = init_timestamps(philo, simu->timestamp)) != SUCCESS)
		return err;
	if (pthread_mutex_init(&(philo->philo_m), NULL) != SUCCESS)
		return error_msg("mutex initialisation for philosopher failed\n", SYS_ERROR);
	simu->philos[id] = philo;
	if ((err = pthread_create(&(simu->threads[id]), NULL, &routine, (void *)philo)) != 0)
		   printf("\ncan't create thread :[%d]", err);
	return SUCCESS;
}