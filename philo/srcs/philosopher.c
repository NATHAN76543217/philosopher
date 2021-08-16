/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:17:29 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 18:23:02 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Check if simulation should be stop
*/
int	shouldStopSimu(const t_philo *philo)
{
	if ((!philo->simu->running || !philo->alive))
		log_philo("should stop", philo);
	return (!philo->simu->running || !philo->alive);
}

/*
** Warn other philosophers to stop the simulation
*/
static void	stop_simulation(const t_philo *philo)
{
	pthread_mutex_lock(&(philo->simu->simu_m));
	philo->simu->running = FALSE;
	log_philo("died.", philo);
	pthread_mutex_unlock(&(philo->simu->simu_m));
	log_philo("End of simulation.", philo);
}

/*
** End of philosopher's lifecycle
*/
static void	routine_ending(t_philo *philo, int i)
{
	if (i == EATING)
	{
		pthread_mutex_unlock(&(philo->simu->forks[philo->left_fork_id]));
		pthread_mutex_unlock(&(philo->simu->forks[philo->right_fork_id]));
	}
	if (philo->simu->running)
		stop_simulation(philo);
}

/*
** Philosopher's lifecycle
*/
void	*routine(void *philosopher)
{
	int			i;
	int			ret;
	t_philo		*philo;

	i = 0;
	philo = (t_philo *) philosopher;
	log_philo("is starting", philo);
	while (!shouldStopSimu(philo))
	{
		ret = philo->activity[i](philo);
		if (ret != SUCCESS)
		{
			if (i == THINKING && ret == EXIT_FAILURE)
				break ;
			pthread_mutex_lock(&(philo->philo_m));
			philo->alive = FALSE;
			pthread_mutex_unlock(&(philo->philo_m));
		}
		i = (i + 1) % 3;
	}
	routine_ending(philo, i);
	return (philo);
}
