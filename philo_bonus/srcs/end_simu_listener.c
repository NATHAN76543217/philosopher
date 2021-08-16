/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_simu_listener.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 13:43:44 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 14:36:22 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** wait for the end of simulation's signal 
** then kill the philosopher and propagates signal
** to others philos
*/
static void	*end_simu_listener(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	log_philo("listener started", philo);
	if (sem_wait(philo->simu->sem[STOP_SIMU]) != SUCCESS)
	{
		log_syserror("critical system error in semaphore (stop_simu)", philo);
		return (NULL);
	}
	log_philo("signal 'end of simulation' received", philo);
	philo->alive = FALSE;
	if (sem_post(philo->simu->sem[STOP_SIMU]) != SUCCESS)
	{
		log_syserror("critical system error in semaphore (stop_simu)", philo);
		return (NULL);
	}
	log_philo("end_simu listener stopped.", philo);
	return (NULL);
}

/*
** start a listener that wait for the end of the simulation
*/
int	start_end_simu_listener(t_philo *philo)
{
	int			err;
	pthread_t	listener_id;

	err = pthread_create(&listener_id, NULL, &end_simu_listener, (void *)philo);
	if (err != SUCCESS)
		return (log_syserror("can't create end_simu listener", philo));
	if (pthread_detach(listener_id) != SUCCESS)
		return (log_syserror("can't detach end_simu listener", philo));
	return (SUCCESS);
}
