#include "philo_bonus.h"

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
		if (sem_post(philo->simu->eat_enough) != SUCCESS)
		{
			printf("%ld %3d   critical system error in semaphore (eat_enough)\n", elapsedStart(philo->timestamp), philo->id);	
			return (SYS_ERROR);
		}
	}	
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
** Take a fork in the middle of the table
*/
static int	take_fork(t_philo *philo)
{
	if (sem_wait(philo->simu->forks) != SUCCESS)
	{
		log_philo("critical system error in semaphore (forks)", philo);
		return SYS_ERROR;
	}
	log_philo("has taken a fork.", philo);
	return (SUCCESS);
}

/*
** picking forks philosopher's phase
*/
int			take_forks(t_philo *philo)
{
	log_philo("is thinking.", philo);
	if (take_fork(philo))
		return (SYS_ERROR);
	if (!philo->alive)
	{
		if (sem_post(philo->simu->forks) != SUCCESS)
		{
			printf("%ld %3d   critical system error in semaphore (forks)\n", elapsedStart(philo->timestamp), philo->id);	
			return (SYS_ERROR);
		}	
		log_philo("has release a fork.", philo);
		return (EXIT_FAILURE);
	}
	if (take_fork(philo))
	{
		if (sem_post(philo->simu->forks) != SUCCESS)
		{
			printf("%ld %3d   critical system error in semaphore (forks)\n", elapsedStart(philo->timestamp), philo->id);	
			return (SYS_ERROR);
		}
		log_philo("has release a fork.", philo);
		return (SYS_ERROR);
	}
	return (SUCCESS);
}
