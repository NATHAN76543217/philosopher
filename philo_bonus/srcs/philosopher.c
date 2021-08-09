#include "philo_bonus.h"


/*
** Warn other philosophers to stop the simulation
*/
static void	stop_simulation(t_philo *philo)
{
	sem_t *forks;
	
	if ((forks = sem_open("simu_m", 0)) == SEM_FAILED)
	{
		printf("Error while opening simu_m\n");
		return ;
	}
	sem_wait(philo->simu->simu_m);
	philo->simu->running = FALSE;
	if ( philo->simu->max_eating != -1 && philo->eat_count < philo->simu->max_eating)
		printf("%ld %d died. %d/%d\n", elapsedStart(philo), philo->id, philo->eat_count, philo->simu->max_eating);
	else if ( philo->simu->max_eating != -1 )
		printf("%ld %d number of eating max reached. %d/%d\n", elapsedStart(philo), philo->id, philo->eat_count, philo->simu->max_eating);
	else
		printf("%ld %d died.\n", elapsedStart(philo), philo->id);
	sem_post(philo->simu->simu_m);
	#ifdef DEBUG
		printf("End of simulation. for (%d)\n", philo->id);
	#else
		printf("End of simulation.\n");
	#endif
	//TODO secure close
	sem_close(forks);
	printf("simu_m closed\n");
}

/*
** Philosopher lifecycle
*/
static void	routine(t_philo *philo)
{
	int			ret_val;
	
	while(TRUE)
	{
		printf("%ld %d is thinking.\n", elapsedStart(philo), philo->id);
		#ifdef DEBUG
			// printf("%ld %d forks as value[%d]\n", elapsedStart(philo), philo->id, philo->forks_in_hand, ret_val);
		#endif
		if (takeFork(philo) != SUCCESS )
		{
			printf("errno = %s\n", strerror(errno));
			continue ;
		}
		ret_val = philo_eat(philo);
		sem_post(philo->simu->forks);
		sem_post(philo->simu->forks);
		philo->forks_in_hand = 0;
		#ifdef DEBUG
			printf("%ld %d has release two forks[have: %d]\n", elapsedStart(philo), philo->id, philo->forks_in_hand);
		#endif
		if ( ret_val == END_SIMULATION || philo_sleep(philo) == END_SIMULATION)
			break ;
	}
	//TODO check if
	if (philo->simu->running)
		stop_simulation(philo);
	destroy_philosopher(philo);
	exit(EXIT_SUCCESS);
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
	#ifdef DEBUG
		printf("philosopher %d destroyed.\n", philo->id);
	#endif
	return ;
}

/*
** init and launch a philosopher process
*/
int     	create_philosopher(t_philo_simu *simu, int id)
{
	int			err;
	t_philo		philo;

	philo.id = id + 1;
	philo.simu = simu;
	philo.eat_count = 0;
	philo.forks_in_hand = 0;
	if ((err = init_timestamps(&philo)) != SUCCESS)
		return err;
	#ifdef DEBUG
		printf("start philosopher %d\n", philo.id);
	#endif
	if ((simu->philos[id] = fork()) == 0)
		routine(&philo);
	else if (simu->philos[id] < 0)
		return error_msg("Error while forking processes.", SYS_ERROR);
	return SUCCESS;
}