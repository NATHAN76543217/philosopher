/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosopher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:03:26 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 18:22:36 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Initialise all timestamp values in a philosopher
*/
static int	init_timestamps(t_philo *philo, struct timeval start_timestamp)
{
	struct timeval	*timestamp[2];

	timestamp[0] = (struct timeval *) malloc(sizeof(struct timeval));
	timestamp[1] = (struct timeval *) malloc(sizeof(struct timeval));
	if (timestamp[0] == NULL || timestamp[1] == NULL)
		return (error_msg("A memory error happen when malloc (philo)\n", \
			MEMORY_ERROR));
	philo->timestamp = ft_memcpy((void *) timestamp[0], \
		(void *) &start_timestamp, \
		sizeof(struct timeval));
	philo->last_meal = ft_memcpy((void *) timestamp[1], \
		(void *) &start_timestamp, \
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
** Init and launch a philosopher thread
*/
static int	create_philosopher_next(t_philo_simu *simu, t_philo *philo)
{
	int	err;

	init_activities(philo);
	err = init_timestamps(philo, simu->timestamp);
	if (err != SUCCESS)
		return (err);
	if (pthread_mutex_init(&(philo->philo_m), NULL) != SUCCESS)
		return (error_msg("mutex initialisation for philosopher failed\n", \
			SYS_ERROR));
	simu->philos[philo->id - 1] = philo;
	err = pthread_create(&(simu->threads[philo->id - 1]), \
		NULL, \
		&routine, \
		(void *)philo);
	if (err != 0)
		printf("\ncan't create thread :[%d]", err);
	return (SUCCESS);
}

/*
** init and launch a philosopher thread
*/
int	create_philosopher(t_philo_simu *simu, int id)
{
	t_philo		*philo;

	philo = (t_philo *) malloc(sizeof(t_philo));
	if (philo == NULL)
		return (error_msg("A memory error happen when malloc\n", MEMORY_ERROR));
	philo->id = id + 1;
	philo->simu = simu;
	philo->eat_count = 0;
	philo->alive = TRUE;
	if (id % 2)
	{
		philo->left_fork_id = id;
		philo->right_fork_id = (id + 1) % simu->number_of_philosopher;
	}	
	else
	{
		philo->right_fork_id = id;
		philo->left_fork_id = (id + 1) % simu->number_of_philosopher;
	}
	if (simu->number_of_philosopher == 1)
		philo->left_fork_id = 1;
	return (create_philosopher_next(simu, philo));
}
