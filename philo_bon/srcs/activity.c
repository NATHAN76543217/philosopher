#include "philo_bon.h"

/*
** Update the last_meal timestamp to now.
*/
static int	updateLastMeal(t_philo *philo)
{
	gettimeofday(&(philo->last_meal), NULL);
	return (SUCCESS);
}

/*
** Eating phase of a philosopher
*/
int			philo_eat(t_philo *philo)
{
	int err;

	log_philo("is eating", philo);
	if ((err = updateLastMeal(philo)) != SUCCESS)
		return (err);
	usleep(philo->simu->time_to_eat * 1000);
	if ((err = updateLastMeal(philo)) != SUCCESS)
		return (err);
	philo->eat_count++;
	if (philo->eat_count == philo->simu->max_eating)
	{
		sem_post(philo->simu->eat_enough);
	}	
	sem_post(philo->simu->forks);
	sem_post(philo->simu->forks);
	printf("%ld %3d  has release two forks.\n", elapsedStart(philo->timestamp), philo->id);
	return (SUCCESS);
}

/*
** Sleeping phase of a philosopher
*/
int			philo_sleep(t_philo *philo)
{
	log_philo("is sleeping", philo);
	usleep(philo->simu->time_to_sleep * 1000);
	return SUCCESS;
}

/*
** Take the fork specified by the id passed in param
*/
static int	take_fork(t_philo *philo)
{
	sem_wait(philo->simu->forks);
	printf("%ld %3d  has taken a fork.\n", elapsedStart(philo->timestamp), philo->id);
	return (SUCCESS);
}

/*
** Getting forks philosopher's phase
*/
int			take_forks(t_philo *philo)
{
	log_philo("is thinking.", philo);
	if (take_fork(philo))
		return (SYS_ERROR);
	if (!philo->alive)
	{
		sem_post(philo->simu->forks);
		return (EXIT_FAILURE);
	}
	if (take_fork(philo))
	{
		sem_post(philo->simu->forks);
		return (SYS_ERROR);
	}
	return (SUCCESS);
}
