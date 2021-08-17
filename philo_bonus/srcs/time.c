/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 12:03:53 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 19:15:52 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** Update the given timestamp to now.
*/
int	updateToNow(struct timeval *timestamp)
{
	gettimeofday(timestamp, NULL);
	return (SUCCESS);
}

/*
** Return the number of milliseconds elapsed since the timestamp passed in params
*/
long	elapsedSince(const struct timeval timestamp)
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
