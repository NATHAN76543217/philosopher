#include "philo_bonus.h"

/*
** Check if simulation should be stop
*/
static int	shouldStopSimu(const t_philo *philo)
{
	if (elapsedLastMeal(philo) > philo->simu->time_to_die)
	{
		#ifdef DEBUG
			printf("%ld %d is starving.\n", elapsedStart(philo), philo->id);
		#endif
		return TRUE;
	}
	return !philo->simu->running;
}

/*
** Eating phase of a philosopher
*/
int			philo_eat(t_philo *philo)
{
	if ( shouldStopSimu(philo) )
		return END_SIMULATION;
	printf("%ld %d is eating\n", elapsedStart(philo), philo->id);
	usleep(philo->simu->time_to_eat * 1000);
	gettimeofday(philo->last_meal, NULL);
	philo->eat_count++;
	if (philo->eat_count == philo->simu->max_eating)
	{
		#ifdef DEBUG
			printf("%ld %d has eat %d times. %d/%d\n", elapsedStart(philo), philo->id, philo->eat_count, philo->eat_count, philo->simu->max_eating);
		#endif
		return END_SIMULATION;
	}	
	return SUCCESS;
}

/*
** Sleeping phase of a philosopher
*/
int			philo_sleep(const t_philo *philo)
{
	printf("%ld %d is sleeping\n", elapsedStart(philo), philo->id);
	usleep(philo->simu->time_to_sleep * 1000);
	if ( shouldStopSimu(philo) )
		return END_SIMULATION;
	return SUCCESS;
}

/*
** Getting forks philosopher's phase
*/
int			takeFork(t_philo *philo)
{
	printf("take fork\n");
	sem_t *forks = NULL;
	
	if ((forks = sem_open("forks", 0)) == SEM_FAILED)
		return error_msg("Error while opening forks\n", SYS_ERROR);
	printf("forks opened\n");
	while (philo->forks_in_hand < 2)
	{
		if (sem_wait(philo->simu->forks) == SUCCESS)
		{
			#ifdef DEBUG
				if ( philo->forks_in_hand == 1)
					printf("%ld %d has taken a second fork[%d]\n", elapsedStart(philo), philo->id, philo->forks_in_hand );
				else
					printf("%ld %d has taken a fork[%d]\n", elapsedStart(philo), philo->id, philo->forks_in_hand);
			#else
				printf("%ld %d has taken a fork\n", elapsedStart(philo), philo->id);
			#endif
			philo->forks_in_hand += 1;
		}
		else
		{
			sem_close(forks);
			printf("forks closed\n");
			return EXIT_FAILURE;
		}
	}
	sem_close(forks);
	printf("forks closed\n");
	return SUCCESS;
}
