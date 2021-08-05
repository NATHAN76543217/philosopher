#include "philo.h"

/*
** Check if simulation should be stop
*/

static int	shouldStopSimu(t_philo *philo)
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

int			philo_sleep(t_philo *philo)
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
	if (!pthread_mutex_lock(&(philo->simu->forks[philo->right_fork_id]) ))
	{
		#ifdef DEBUG
			printf("%ld %d has taken a fork[%d]\n", elapsedStart(philo), philo->id, philo->right_fork_id);
		#else
			printf("%ld %d has taken a fork\n", elapsedStart(philo), philo->id);
		#endif
	}
	else
		return EXIT_FAILURE;
	if (!pthread_mutex_lock(&(philo->simu->forks[philo->left_fork_id])))
	{
		#ifdef DEBUG
			printf("%ld %d has taken a second fork[%d]\n", elapsedStart(philo), philo->id, philo->left_fork_id);
		#else
			printf("%ld %d has taken a fork\n", elapsedStart(philo), philo->id);
		#endif
		return SUCCESS;
	}
	return EXIT_FAILURE;
}
