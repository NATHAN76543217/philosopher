/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_philosopher.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:34:10 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 19:28:52 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Free a philosopher structure
*/
static void	destroy_philosopher(t_philo *philo)
{
	if (!philo)
		return ;
	log_philo("structure destroyed", philo);
	free(philo);
	return ;
}

/*
** Free all philospher structures in the simulation
*/
void	destroy_all_philosophers(t_philo_simu *simu)
{
	int	i;

	i = 0;
	while (i < simu->number_of_philosopher)
	{
		destroy_philosopher(simu->philos[i]);
		i++;
	}
	return ;
}
