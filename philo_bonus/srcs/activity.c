/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 15:51:07 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 19:37:15 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** Suspend process for a certain amount of time 
** in non-blocking manner
*/
static int	waiting(t_philo *philo, int ms)
{
	updateToNow(&(philo->start_activity));
	while (elapsedSince(philo->start_activity) < ms)
	{
		if (philo->alive == FALSE)
			break ;
		usleep(CHECK_DELAY * 1000);
	}
	return (SUCCESS);
}

/*
** Eating phase of a philosopher
*/
int	philo_eat(t_philo *philo)
{
	int	err;

	log_philo("is eating.", philo);
	err = updateToNow(&(philo->last_meal));
	if (err != SUCCESS)
		return (err);
	waiting(philo, philo->simu->time_to_eat);
	philo->eat_count++;
	if (philo->eat_count == philo->simu->max_eating)
	{
		if (sem_post(philo->simu->sem[EAT_ENOUGH]) != SUCCESS)
			return (log_syserror("critical system error in semaphore (forks)", \
				philo));
	}	
	if (sem_post(philo->simu->sem[FORKS]) != SUCCESS)
		return (log_syserror("critical system error in semaphore (forks)", \
			philo));
	if (sem_post(philo->simu->sem[FORKS]) != SUCCESS)
		return (log_syserror("critical system error in semaphore (forks)", \
			philo));
	log_philo("has release two forks.", philo);
	return (SUCCESS);
}

/*
** Sleeping phase of a philosopher
*/
int	philo_sleep(t_philo *philo)
{
	log_philo("is sleeping", philo);
	waiting(philo, philo->simu->time_to_sleep);
	return (SUCCESS);
}

/*
** Take a fork in the middle of the table
*/
static int	take_fork(t_philo *philo)
{
	if (sem_wait(philo->simu->sem[FORKS]) != SUCCESS)
		return (log_syserror("critical system error in semaphore (forks)", \
			philo));
	log_philo("has taken a fork.", philo);
	return (SUCCESS);
}

/*
** picking forks philosopher's phase
*/
int	take_forks(t_philo *philo)
{
	log_philo("is thinking.", philo);
	if (take_fork(philo))
		return (SYS_ERROR);
	if (!philo->alive)
	{
		if (sem_post(philo->simu->sem[FORKS]) != SUCCESS)
			return (log_syserror("critical system error in semaphore (forks)", \
				philo));
		log_philo("has release a fork.", philo);
		return (EXIT_FAILURE);
	}
	if (take_fork(philo))
	{
		if (sem_post(philo->simu->sem[FORKS]) != SUCCESS)
			return (log_syserror("critical system error in semaphore (forks)", \
				philo));
		log_philo("has release a fork.", philo);
		return (SYS_ERROR);
	}
	return (SUCCESS);
}
