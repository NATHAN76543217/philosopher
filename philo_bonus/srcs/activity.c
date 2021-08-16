/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 15:51:07 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 15:54:22 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

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
int	philo_eat(t_philo *philo)
{
	int	err;

	log_philo("is eating.", philo);
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
	usleep(philo->simu->time_to_sleep * 1000);
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
