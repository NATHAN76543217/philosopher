/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosopher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 13:16:11 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 13:18:24 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** Initialise all timestamp values in a philosopher to 'start_timestamp'
*/
static int	init_timestamps_to(t_philo *philo, struct timeval start_timestamp)
{
	ft_memcpy(
		(void *) &(philo->timestamp),
		(void *) &start_timestamp,
		sizeof(struct timeval));
	ft_memcpy(
		(void *) &(philo->last_meal),
		(void *) &start_timestamp,
		sizeof(struct timeval));
	return (SUCCESS);
}

/*
** init the array of pointers on function
*/
static void	init_activities(t_philo *philo)
{
	philo->activity[THINKING] = &take_forks;
	philo->activity[EATING] = &philo_eat;
	philo->activity[SLEEPING] = &philo_sleep;
}

/*
** init and launch a philosopher process
*/
int	create_philosopher(t_philo_simu *simu, int id)
{
	t_philo		philo;

	philo.id = id + 1;
	philo.simu = simu;
	philo.eat_count = 0;
	philo.alive = TRUE;
	philo.current = 0;
	init_timestamps_to(&philo, simu->timestamp);
	init_activities(&philo);
	simu->philos_id[id] = fork();
	if (simu->philos_id[id] == SUCCESS)
		philosopher(&philo);
	else if (simu->philos_id[id] < 0)
		return (error_msg("Error while forking processes.", SYS_ERROR));
	return (SUCCESS);
}
