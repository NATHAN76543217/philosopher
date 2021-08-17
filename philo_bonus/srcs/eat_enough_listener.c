/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_enough_listener.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 15:50:47 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 19:18:12 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** Handle the listener unlocking
** Stop simulation if all philosophers have eaten enough 
** and the simulation isn't already stopped 
*/
static void	*listener_unlocked(t_philo_simu *simu)
{
	if (simu->running == FALSE)
	{
		log_simu("eat_enough listener stopped.", simu);
		return (NULL);
	}	
	log_simu("all the philosophers have eaten enough.", simu);
	if (sem_post(simu->sem[STOP_SIMU]) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (stop_simu)\n", \
			elapsedSince(simu->timestamp));
		return (NULL);
	}
	log_simu("eat_enough listener stopped.", simu);
	return (NULL);
}

/*
** wait that all philosophers have eaten enough
*/
static void	*eat_enough_listener(void *simulation)
{
	t_philo_simu	*simu;

	simu = (void *)simulation;
	log_simu("eat enough listener started.", simu);
	while (simu->count_philo_eat_enough < simu->number_of_philosopher
		&& simu->running == TRUE)
	{
		if (sem_wait(simu->sem[EAT_ENOUGH]) != SUCCESS)
		{
			printf("%ld   -   critical system error in semaphore (%s)\n", \
				elapsedSince(simu->timestamp), \
				"eat_enough");
			return (NULL);
		}
		simu->count_philo_eat_enough ++;
	}
	return (listener_unlocked(simu));
}

/*
** stop and join eat_enough listener
*/
int	stop_eat_enough_listener(t_philo_simu *simu)
{
	if (sem_post(simu->sem[EAT_ENOUGH]) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (eat_enought)\n", \
			elapsedSince(simu->timestamp));
		return (SYS_ERROR);
	}
	if (pthread_join(simu->listener_id, NULL) != SUCCESS)
	{
		log_simu("failed to join thtread eat_enough listener", simu);
		return (SYS_ERROR);
	}
	log_simu("eat_enough listener joined.", simu);
	return (SUCCESS);
}

/*
** start a listener waiting for all the philosophers to have eaten enough
*/
int	start_eat_enough_listener(t_philo_simu *simu)
{
	int	err;

	err = pthread_create(&(simu->listener_id), \
		NULL, \
		&eat_enough_listener, \
		(void *)simu);
	if (err != 0)
		return (error_msg("can't create eat_enough listener", SYS_ERROR));
	return (SUCCESS);
}
