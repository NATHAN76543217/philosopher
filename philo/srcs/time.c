/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 17:42:35 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 17:43:53 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Return the number of milliseconds elapsed since the timestamp passed in param
*/
long	elapsedStart(const struct timeval timestamp)
{
	long			last[2];
	long			now[2];
	struct timeval	tInit;

	gettimeofday(&tInit, NULL);
	last[0] = (long) timestamp.tv_sec;
	last[1] = (long) timestamp.tv_usec;
	now[0] = (long) tInit.tv_sec;
	now[1] = (long) tInit.tv_usec;
	return (((now[0] - last[0]) * 1000000 + (now[1] - last[1])) / 1000);
}

/*
** Return the number of milliseconds elapsed since the last meal
*/
long	elapsedLastMeal(const t_philo *const philo)
{
	long			last[2];
	long			now[2];
	struct timeval	tInit;

	gettimeofday(&tInit, NULL);
	last[0] = (long) philo->last_meal->tv_sec;
	last[1] = (long) philo->last_meal->tv_usec;
	now[0] = (long) tInit.tv_sec;
	now[1] = (long) tInit.tv_usec;
	return (((now[0] - last[0]) * 1000000 + (now[1] - last[1])) / 1000);
}
