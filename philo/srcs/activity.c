#include "philo.h"

/*
** Update the last_meal timestamp to now.
*/
static int	updateLastMeal(t_philo *philo)
{
	if (pthread_mutex_lock(&(philo->philo_m)) != SUCCESS)
		return (error_msg("mutex locking system error\n", SYS_ERROR));
	gettimeofday(philo->last_meal, NULL);
	if (pthread_mutex_unlock(&(philo->philo_m)) != SUCCESS)
		return (error_msg("mutex locking system error\n", SYS_ERROR));
	return (SUCCESS);
}

/*
** Eating phase of a philosopher
*/
int	philo_eat(t_philo *philo)
{
	int	err;

	log_philo("is eating", philo);
	err = updateLastMeal(philo);
	if (err != SUCCESS)
		return (err);
	usleep(philo->simu->time_to_eat * 1000);
	err = updateLastMeal(philo);
	if (err != SUCCESS)
		return (err);
	philo->eat_count++;
	if (philo->eat_count == philo->simu->max_eating)
	{
		pthread_mutex_lock(&(philo->simu->simu_m));
		philo->simu->count_philo_eat_enought++;
		pthread_mutex_unlock(&(philo->simu->simu_m));
	}	
	pthread_mutex_unlock(&(philo->simu->forks[philo->left_fork_id]));
	pthread_mutex_unlock(&(philo->simu->forks[philo->right_fork_id]));
	printf("%ld %3d  has release two forks[%d][%d]\n", \
		elapsedStart(*(philo->timestamp)), \
		philo->id, philo->left_fork_id, philo->right_fork_id);
	return (SUCCESS);
}

/*
** Sleeping phase of a philosopher
*/
int	philo_sleep(t_philo *philo)
{
	log_philo("is sleeping", philo);
	usleep(philo->simu->time_to_sleep * 1000);
	return (SUCCESS);
}

/*
** Take the fork specified by the id passed in param
*/
static int	take_fork(t_philo *philo, int fork_id)
{
	if (pthread_mutex_lock(&(philo->simu->forks[fork_id])))
		return (error_msg("mutex locking system error\n", SYS_ERROR));
	printf("%ld %3d  has taken a fork. [%d]\n", \
		elapsedStart(*(philo->timestamp)), \
		philo->id, fork_id);
	return (SUCCESS);
}

/*
** Getting forks philosopher's phase
*/
int	take_forks(t_philo *philo)
{
	log_philo("is thinking.", philo);
	if (take_fork(philo, philo->right_fork_id))
		return (SYS_ERROR);
	if (shouldStopSimu(philo))
	{
		pthread_mutex_unlock(&(philo->simu->forks[philo->right_fork_id]));
		return (EXIT_FAILURE);
	}
	if (take_fork(philo, philo->left_fork_id))
	{
		pthread_mutex_unlock(&(philo->simu->forks[philo->right_fork_id]));
		return (SYS_ERROR);
	}
	return (SUCCESS);
}
