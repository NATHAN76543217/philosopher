/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 13:45:22 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 13:21:38 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
**	static int	check_param_validity_next(t_philo_simu const *simu)
**	{
**		if (simu->max_eating < -1 || simu->max_eating > MAX_EAT)
**			return (error_msg("You cannot have a " \
**				"'number_of_times_each_philosopher_must_eat' value " \
**				"less than -1 or greater than " \
**				STRI(MAX_EAT) ".\n", ARGUMENT_ERROR));
**		else if (simu->max_eating == 0)
**			return (error_msg("You cannot have a " \
**				"'number_of_times_each_philosopher_must_eat' value " \
**				"equal to 0. You can disable this option with the value -1."\
**				STRI(MAX_EAT) ".\n", ARGUMENT_ERROR));
**		else if (simu->nb_fork_init < 2)
**			return (error_msg("You cannot have a number of forks less than 2." \
**				"\nThis behavior is unexpected, please report us the problem.\n", \
**				SYS_ERROR));
**		return (SUCCESS);
**	}
*/
/*
** Check if the simulation's settings are correct 
*/
static int	check_param_validity_next(t_philo_simu const *simu)
{
	if (simu->max_eating < -1 || simu->max_eating > MAX_EAT_LIMIT)
		return (error_msg("You cannot have a " \
			"'number_of_times_each_philosopher_must_eat' value " \
			"less than -1 or greater than 100.\n", ARGUMENT_ERROR));
	else if (simu->max_eating == 0)
		return (error_msg("You cannot have a " \
			"'number_of_times_each_philosopher_must_eat' value " \
			"equal to 0. You can disable this option " \
			"with the value -1.\n", ARGUMENT_ERROR));
	else if (simu->nb_fork_init < 2)
		return (error_msg("You cannot have a number of forks " \
			"less than 2.\nThis behavior is unexpected, " \
			"please report us the problem.\n", SYS_ERROR));
	return (SUCCESS);
}

/*
**	static int	check_param_validity(t_philo_simu const *simu)
**	{
**		if (simu->number_of_philosopher <= 0
**			|| simu->number_of_philosopher > MAX_PHILO)
**			return (error_msg("You cannot have less than 1 philosopher " \
**				" or more than " STRI(MAX_PHILO) ".\n", ARGUMENT_ERROR));
**		else if (simu->time_to_die <= 0 || simu->time_to_die > MAX_TIME)
**			return (error_msg("You cannot have a 'time_to_die' value " \
**				"less or equal than 0 or greater than " \
**				STRI(MAX_TIME) ".\n", ARGUMENT_ERROR));
**		else if (simu->time_to_eat <= 0 || simu->time_to_eat > MAX_TIME)
**			return (error_msg("You cannot have a 'time_to_eat' value " \
**				"less or equal than 0 or greater than " \
**				STRI(MAX_TIME) ".\n", ARGUMENT_ERROR));
**		else if (simu->time_to_sleep <= 0 || simu->time_to_sleep > MAX_TIME)
**			return (error_msg("You cannot have a 'time_to_sleep' value " \
**				" less or equal than 0 or greater than " \
**				STRI(MAX_TIME) ".\n", ARGUMENT_ERROR));
**		return (check_param_validity_next(simu));
**	}*/
/*
** Check if the simulation's settings are correct 
*/
static int	check_param_validity(t_philo_simu const *simu)
{
	if (simu->number_of_philosopher <= 0
		|| simu->number_of_philosopher > MAX_PHILO)
		return (error_msg("You cannot have less than 1 philosopher " \
			" or more than 100 philosophers.\n", ARGUMENT_ERROR));
	else if (simu->time_to_die <= 0 || simu->time_to_die > MAX_SIMU_TIME)
		return (error_msg("You cannot have a 'time_to_die' value " \
			"less or equal than 0 or greater than 30000.\n", ARGUMENT_ERROR));
	else if (simu->time_to_eat <= 0 || simu->time_to_eat > MAX_EAT_TIME)
		return (error_msg("You cannot have a 'time_to_eat' value " \
			"less or equal than 0 or greater than 10000.\n", ARGUMENT_ERROR));
	else if (simu->time_to_sleep <= 0 || simu->time_to_sleep > MAX_SLEEP_TIME)
		return (error_msg("You cannot have a 'time_to_sleep' value " \
			" less or equal than 0 or greater than 10000.\n", ARGUMENT_ERROR));
	return (check_param_validity_next(simu));
}

/*
** Init simulation's semaphores
*/
static int	semaphores_initialisation(t_philo_simu *simu)
{
	int		i;
	char	*sem_names[4];

	i = FORKS;
	sem_names[0] = "forks";
	sem_names[1] = "eat_enough";
	sem_names[2] = "stop_simu";
	sem_names[3] = "writing";
	while (i <= WRITING)
	{
		sem_unlink(sem_names[i]);
		simu->sem[i] = sem_open(sem_names[i], O_CREAT | O_EXCL, S_IRWXU, \
			simu->nb_fork_init);
		if (simu->sem[i] == SEM_FAILED)
		{
			while (--i >= 0)
				sem_close(simu->sem[i]);
			return (error_msg("semaphore initialisation failed\n", SYS_ERROR));
		}
		i++;
	}
	log_simu("semaphores initialised", simu);
	return (SUCCESS);
}

/*
** Init simulation with program params
*/
int	init_simulation_next(t_philo_simu *const simu)
{
	int	err;

	gettimeofday(&(simu->timestamp), NULL);
	simu->philos_id = \
		(pid_t *) malloc(sizeof(pid_t) * simu->number_of_philosopher);
	if (simu->philos_id == NULL)
		return (error_msg("A memory error happen when malloc (simu)\n", \
			MEMORY_ERROR));
	err = check_param_validity(simu);
	if (err != SUCCESS)
		return (err);
	err = semaphores_initialisation(simu);
	if (err != SUCCESS)
		return (err);
	game_info(simu);
	return (SUCCESS);
}

/*
** Init simulation with program params
*/
int	init_simulation(int ac, char **av, t_philo_simu *const simu)
{
	if (ac < 5 || ac > 6 )
		return (error_msg("Bad number of arguments\n\n\tUsage:\t./philo "\
			"number_of_philosophers time_to_die time_to_eat time_to_sleep "\
			"[number_of_times_each_philosopher_must_eat]\n\n", ARGUMENT_ERROR));
	memset(simu, 0, sizeof(t_philo_simu));
	simu->number_of_philosopher = ft_atoi(av[1]);
	simu->time_to_die = ft_atoi(av[2]);
	simu->time_to_eat = ft_atoi(av[3]);
	simu->time_to_sleep = ft_atoi(av[4]);
	simu->count_philo_eat_enough = 0;
	simu->running = TRUE;
	if (simu->number_of_philosopher == 1)
		simu->nb_fork_init = 2;
	else
		simu->nb_fork_init = simu->number_of_philosopher;
	if (ac == 6)
		simu->max_eating = ft_atoi(av[5]);
	else
		simu->max_eating = -1;
	return (init_simulation_next(simu));
}
