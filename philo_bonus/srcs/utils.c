/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 12:01:50 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 21:04:59 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** The isdigit() function tests for a decimal digit character.
*/
static int	ft_isdigit(const char c)
{
	return (c >= '0' && c <= '9');
}

/*
** The ft_atoi() function converts the initial portion of the string pointed
** to by str to int representation.
*/
int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (*(str + i) == ' ' || (*(str + i) >= '\t' && *(str + i) <= '\r'))
		i++;
	if (*(str + i) == '+' || *(str + i) == '-')
	{
		if (*(str + i++) == '-')
			sign = -1;
	}
	while (ft_isdigit(*(str + i)))
	{
		if (result * sign > INT32_MAX)
			return (-1);
		else if (result * sign < INT32_MIN)
			return (0);
		result = result * 10 + (*(str + i++) - '0');
	}
	result *= sign;
	return (result);
}

/*
** copy memory area values to another area
*/
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;
	size_t	i;

	i = 0;
	while (i < n)
	{
		s = (char *) src + i;
		d = (char *) dest + i;
		*d = *s;
		i++;
	}
	return (dest);
}

/*
** Dump simulation config
*/
void	game_info(t_philo_simu const *simu)
{
	printf("\n---- philo_bonus simulation ----\n\n");
	printf("number_of_philosophers = %d\n", simu->number_of_philosopher);
	printf("time_to_die  = %d\n", simu->time_to_die);
	printf("time_to_eat = %d\n", simu->time_to_eat);
	printf("time_to_sleep = %d\n", simu->time_to_sleep);
	if (simu->max_eating < 0)
		printf("[number_of_times_each_philosopher_must_eat] = (disactived)\n");
	else
		printf("[number_of_times_each_philosopher_must_eat] = %d\n", \
			simu->max_eating);
	printf("nb_fork = %d\n", simu->nb_fork_init);
	dprintf(1, "Simulation initialisation done.\n\n------\n\n");
	return ;
}
