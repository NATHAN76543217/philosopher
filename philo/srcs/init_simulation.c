/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 12:45:32 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 13:22:37 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Check if the simulation's settings are correct 
*/
static int	check_param_validity(t_philo_simu const *simu)
{
	if (simu->number_of_philosopher <= 0
		|| simu->number_of_philosopher > MAX_PHILO)
		return (error_msg("You cannot have less than 1 philosopher or " \
			"more than 100 philosophers.\n", ARGUMENT_ERROR));
	else if (simu->time_to_die <= 0 || simu->time_to_die > MAX_SIMU_TIME)
		return (error_msg("You cannot have a 'time_to_die' value less or " \
			"equal than 0 or greater than 30000.\n", ARGUMENT_ERROR));
	else if (simu->time_to_eat <= 0 || simu->time_to_eat > MAX_EAT_TIME)
		return (error_msg("You cannot have a 'time_to_eat' value less or " \
			"equal than 0 or greater than 10000.\n", ARGUMENT_ERROR));
	else if (simu->time_to_sleep <= 0 || simu->time_to_sleep > MAX_SLEEP_TIME)
		return (error_msg("You cannot have a 'time_to_sleep' value less or " \
			"equal than 0 or greater than 10000\n", ARGUMENT_ERROR));
	else if (simu->max_eating < -1 || simu->max_eating > MAX_EAT_LIMIT
		|| simu->max_eating == 0)
		return (error_msg("You cannot have a " \
			"'number_of_times_each_philosopher_must_eat' value less " \
			"than -1 equal to 0 or greater than 100.\n", ARGUMENT_ERROR));
	else if (simu->nb_fork < 2)
		return (error_msg("You cannot have a number of forks less than 2.\n" \
			"This behavior is unexpected, please report us the problem.\n", \
			SYS_ERROR));
	return (SUCCESS);
}

/*
** Initialise simulation's mutex
*/
static int	mutex_initialisation(t_philo_simu *simu)
{
	int	i;

	i = -1;
	while (++i < simu->nb_fork)
		if (pthread_mutex_init(&(simu->forks[i]), NULL) != SUCCESS)
			return (error_msg("mutex initialisation failed\n", SYS_ERROR));
	if (pthread_mutex_init(&(simu->simu_m), NULL) != SUCCESS)
		return (error_msg("mutex initialisation failed\n", SYS_ERROR));
	return (SUCCESS);
}

/*
** Malloc necessary spaces for simulation
*/
static int	malloc_simu(t_philo_simu *simu)
{
	simu->threads = \
		(pthread_t *) malloc(sizeof(pthread_t) * simu->number_of_philosopher);
	simu->forks = \
		(pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * simu->nb_fork);
	simu->philos = \
		(t_philo **) malloc(sizeof(t_philo *) * simu->number_of_philosopher);
	if (simu->threads == NULL
		|| simu->forks == NULL
		|| simu->philos == NULL)
		return (error_msg("A memory error happen when malloc (simu)\n", \
			MEMORY_ERROR));
	return (SUCCESS);
}

int	init_simu_2(t_philo_simu *simu)
{
	int	err;

	if (gettimeofday(&(simu->timestamp), NULL) != SUCCESS)
		return (SYS_ERROR);
	err = malloc_simu(simu);
	if (err != SUCCESS)
		return (err);
	err = check_param_validity(simu);
	if (err != SUCCESS)
		return (err);
	err = mutex_initialisation(simu);
	if (err != SUCCESS)
		return (err);
	start_game_info(simu);
	return (SUCCESS);
}

/*
** Init simulation with program params
*/
int	init_simu(int ac, char **av, t_philo_simu *const simu)
{
	if (ac < 5 || ac > 6 )
		return (error_msg("Bad number of arguments\n\n\tUsage:\t./philo " \
			"number_of_philosophers time_to_die time_to_eat time_to_sleep" \
			"[number_of_times_each_philosopher_must_eat]\n\n", ARGUMENT_ERROR));
	memset(simu, 0, sizeof(t_philo_simu));
	simu->running = TRUE;
	simu->number_of_philosopher = ft_atoi(av[1]);
	simu->time_to_die = ft_atoi(av[2]);
	simu->time_to_eat = ft_atoi(av[3]);
	simu->time_to_sleep = ft_atoi(av[4]);
	simu->count_philo_eat_enought = 0;
	simu->nb_fork = simu->number_of_philosopher;
	if (simu->number_of_philosopher == 1)
		simu->nb_fork = 2;
	simu->max_eating = -1;
	if (ac == 6)
		simu->max_eating = ft_atoi(av[5]);
	return (init_simu_2(simu));
}
