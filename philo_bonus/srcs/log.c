/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 13:27:49 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 14:36:41 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** The log_syserror() function print a philosopher status well formated 
*/
int	log_syserror(char *str, const t_philo *philo)
{
	printf("%ld %3d   %s\n", elapsedStart(philo->timestamp), philo->id, str);
	return (SYS_ERROR);
}

/*
** The log_philo() function print a philosopher status well formated 
*/
void	log_philo(char *str, const t_philo *philo)
{
	if (sem_wait(philo->simu->sem[WRITING]) != SUCCESS)
	{	
		printf("%ld %3d   critical system error in semaphore (writing)\n", \
			elapsedStart(philo->timestamp), \
			philo->id);
		return ;
	}
	printf("%ld %3d   %s\n", elapsedStart(philo->timestamp), philo->id, str);
	if (sem_post(philo->simu->sem[WRITING]) != SUCCESS)
	{
		printf("%ld %3d   critical system error in semaphore (writing)\n", \
			elapsedStart(philo->timestamp), \
			philo->id);
		return ;
	}
}

/*
** The log_simu() function print simulation status well formated 
*/
void	log_simu(char *str, const t_philo_simu *simu)
{
	if (sem_wait(simu->sem[WRITING]) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (writing) %s\n", \
			elapsedStart(simu->timestamp), \
			str);
		return ;
	}
	printf("%ld   -   %s\n", elapsedStart(simu->timestamp), str);
	if (sem_post(simu->sem[WRITING]) != SUCCESS)
	{
		printf("%ld   -   critical system error in semaphore (writing2)%s\n", \
			elapsedStart(simu->timestamp), \
			str);
		return ;
	}
}

/*
** The error_msg() function print a given error before returning a given value 
*/
int	error_msg(const char *const str, const int ret_value)
{
	printf("%s", str);
	return (ret_value);
}
