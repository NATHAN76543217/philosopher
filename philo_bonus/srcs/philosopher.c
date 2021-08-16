/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 12:04:39 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 14:36:22 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
**
*/
static int	routine_ending(t_philo *philo)
{
	if (sem_post(philo->simu->sem[FORKS]) != SUCCESS)
	{
		printf("%ld %3d   critical system error in semaphore (forks)\n", \
			elapsedStart(philo->timestamp), philo->id);
		return (SYS_ERROR);
	}
	if (sem_post(philo->simu->sem[FORKS]) != SUCCESS)
	{
		printf("%ld %3d   critical system error in semaphore (forks)\n", \
			elapsedStart(philo->timestamp), philo->id);
		return (SYS_ERROR);
	}
	log_philo("has release two forks.", philo);
	sem_post(philo->simu->sem[STOP_SIMU]);
	return (SUCCESS);
}

/*
** Philosopher lifecycle
*/
static int	routine(t_philo *philo)
{
	int	i;
	int	ret;

	i = 0;
	while (philo->alive)
	{
		philo->current = i;
		ret = philo->activity[i](philo);
		if (ret != SUCCESS)
		{
			if (i == THINKING && ret == EXIT_FAILURE)
				break ;
			philo->alive = FALSE;
		}
		i = (i + 1) % 3;
	}
	if (i == EATING)
		return (routine_ending(philo));
	return (SUCCESS);
}

/*
** Kill the given philosopher and terminate is process
*/
static void	*kill_philo(t_philo *philo, int ret)
{
	if (pthread_join(philo->monitor_id, NULL) != SUCCESS)
		log_philo("failed to join the thread monitor", philo);
	log_philo("died.", philo);
	exit(ret);
}

/*
** philosophers process entrypoint
*/
void	philosopher(t_philo *philo)
{
	int	ret;

	log_philo("is starting", philo);
	ret = start_monitoring(philo);
	if (ret != SUCCESS)
		kill_philo(philo, ret);
	ret = routine(philo);
	if (ret != SUCCESS)
		kill_philo(philo, ret);
	kill_philo(philo, EXIT_SUCCESS);
	return ;
}
