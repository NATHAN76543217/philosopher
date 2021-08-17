/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 13:26:17 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 19:20:11 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** Signal to the philosopher to die and signal his condition to the simulation
*/
void	set_philo_should_die(t_philo *philo)
{
	printf("%ld %3d   must die last eating = %ld ms\n", \
		elapsedSince(philo->timestamp), \
		philo->id, \
		elapsedSince(philo->last_meal));
	philo->alive = FALSE;
	if (sem_post(philo->simu->sem[STOP_SIMU]) != SUCCESS)
		log_syserror("critical system error in semaphore " \
			"(monitor - stop_simu)", philo);
}

/*
** Check every CHECK_DELAY milliseconds if the philosopher is starving
*/
static void	*monitor(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *) philosopher;
	log_philo("monitor started.", philo);
	while (philo->alive)
	{
		if (elapsedSince(philo->last_meal) > philo->simu->time_to_die)
		{
			set_philo_should_die(philo);
			break ;
		}
		usleep(CHECK_DELAY * 1000);
	}
	log_philo("monitor stopped.", philo);
	return (NULL);
}

static int	start_monitor(t_philo *philo)
{
	int	err;

	err = pthread_create(&(philo->monitor_id), NULL, &monitor, (void *)philo);
	if (err != 0)
		return (log_syserror("can't create thread monitor", philo));
	return (SUCCESS);
}

int	start_monitoring(t_philo *philo)
{
	start_monitor(philo);
	start_end_simu_listener(philo);
	return (SUCCESS);
}
