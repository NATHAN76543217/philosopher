#include "philo.h"


/*
** Warn other philosophers to stop the simulation
*/
static void	stop_simulation(t_philo *philo)
{
	pthread_mutex_lock(&(philo->simu->simu_m));
	philo->simu->running = FALSE;
	if ( philo->simu->max_eating != -1 && philo->eat_count < philo->simu->max_eating)
		printf("%ld %d died. %d/%d\n", elapsedStart(philo), philo->id, philo->eat_count, philo->simu->max_eating);
	else if ( philo->simu->max_eating != -1 )
		printf("%ld %d number of eating max reached. %d/%d\n", elapsedStart(philo), philo->id, philo->eat_count, philo->simu->max_eating);
	else
		printf("%ld %d died.\n", elapsedStart(philo), philo->id);
	pthread_mutex_unlock(&(philo->simu->simu_m));
	#ifdef DEBUG
		printf("End of simulation. for (%d)\n", philo->id);
	#else
		printf("End of simulation.\n");
	#endif
}

/*
** Philosopher lifecycle
*/
static void	*routine(void* philosopher)
{
	int			ret_val;
	t_philo*	philo;
	
	philo = (t_philo *) philosopher;
	while(TRUE)
	{
		printf("%ld %d is thinking.\n", elapsedStart(philo), philo->id);
		if (takeFork(philo) != SUCCESS)
			continue ;
		ret_val = philo_eat(philo);
		pthread_mutex_unlock( &(philo->simu->forks[philo->left_fork_id]) );
		pthread_mutex_unlock( &(philo->simu->forks[philo->right_fork_id]) );
		#ifdef DEBUG
			printf("%ld %d has release two forks[%d][%d]\n", elapsedStart(philo), philo->id, philo->left_fork_id, philo->right_fork_id);
		#endif
		if ( ret_val == END_SIMULATION || philo_sleep(philo) == END_SIMULATION)
			break ;
	}
	if (philo->simu->running)
		stop_simulation(philo);
	return philo;
}

/*
** Initialise all timestamp values in a philosopher
*/
static int	init_timestamps(t_philo *philo)
{
	struct timeval *timestamp[2];

	if (( timestamp[0] = (struct timeval*) malloc( sizeof( struct timeval ))) == NULL
	||	( timestamp[1] = (struct timeval*) malloc( sizeof( struct timeval ))) == NULL )
			return error_msg("A memory error happen when malloc (philo)\n", MEMORY_ERROR);
	gettimeofday(timestamp[0], NULL);
	gettimeofday(timestamp[1], NULL);
	philo->timestamp = timestamp[0];
	philo->last_meal = timestamp[1];
	return SUCCESS;
}

/*
** Free a philosopher structure
*/
void		destroy_philosopher(t_philo *philo)
{
	if (!philo)
		return ;
	if ( philo->timestamp )
		free( philo->timestamp );
	if ( philo->last_meal )
		free( philo->last_meal );
	free(philo);
	#ifdef DEBUG
		printf("philosopher %d destroyed.\n", philo->id);
	#endif
	return ;
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
	philo->right_fork_id = ( id % 2) ? id : (id + 1) % simu->number_of_philosopher;
	philo->left_fork_id = ( id % 2) ? (id + 1) % simu->number_of_philosopher : id;
	if ( simu->number_of_philosopher == 1 )
		philo->left_fork_id = 1;
	if ((err = init_timestamps(philo)) != SUCCESS)
		return err;
	#ifdef DEBUG
		printf("start philosopher %d\n", philo->id);
	#endif
	if ((err = pthread_create(&(simu->philos[id]), NULL, &routine, (void *)philo)) != 0)
		   printf("\ncan't create thread :[%d]", err);
	return SUCCESS;
}