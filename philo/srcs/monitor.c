/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:24:30 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 20:01:15 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_eat_enough(t_philo_simu *simu)
{
	if (simu->count_philo_eat_enought == simu->number_of_philosopher)
	{
		pthread_mutex_lock(&(simu->simu_m));
		simu->running = FALSE;
		pthread_mutex_unlock(&(simu->simu_m));
		log_simu("every philosopher have eat enought", simu);
	}
	return (SUCCESS);
}

static void	*monitor(void *simulation)
{
	int				i;
	t_philo_simu	*simu;

	i = 0;
	simu = (t_philo_simu *) simulation;
	while (simu->running)
	{
		usleep(1000);
		pthread_mutex_lock(&(simu->philos[i]->philo_m));
		if (elapsedSince(simu->philos[i]->last_meal) > simu->time_to_die)
		{
			simu->philos[i]->alive = FALSE;
			simu->running = FALSE;
			printf("%ld %3d  must die cause doesn't have eat since %ld ms\n", \
				elapsedSince(simu->philos[i]->timestamp), \
				simu->philos[i]->id, elapsedSince(simu->philos[i]->last_meal));
			pthread_mutex_unlock(&(simu->philos[i]->philo_m));
			break ;
		}
		pthread_mutex_unlock(&(simu->philos[i]->philo_m));
		check_eat_enough(simu);
		i = (i + 1) % simu->number_of_philosopher;
	}
	return (NULL);
}

int	create_monitor(t_philo_simu *simu)
{
	int	err;

	err = pthread_create(&(simu->monitor), NULL, &monitor, (void *)simu);
	if (err != SUCCESS)
		printf("\ncan't create monitor thread :[%d]", err);
	return (SUCCESS);
}
