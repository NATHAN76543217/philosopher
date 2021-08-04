#include "philo.h"

void	start_game_info(t_philo_simu* simu)
{
	printf("\n---- philo simuation ----\n\n");
	printf("number_of_philosophers = %d\n", simu->number_of_philosopher);
	printf("time_to_die  = %d\n", simu->time_to_die);
	printf("time_to_eat = %d\n", simu->time_to_eat);
	printf("time_to_sleep = %d\n", simu->time_to_sleep);
	( simu->max_eating < 0 )
		? printf("[number_of_times_each_philosopher_must_eat] = \"(disactived)\"")
		: printf("[number_of_times_each_philosopher_must_eat] = %d\n", simu->max_eating);
	printf("nb_fork = %d\n", simu->nb_fork);
	dprintf(1, "Simulation initialisation done.\n\n------\n\n");
	return ;
}

int clear_all(t_philo_simu *simu)
{
	free(simu);
	return 0;
}

int	check_param_validity(t_philo_simu *simu)
{
	if (simu->number_of_philosopher <= 1)
		return error_msg("You cannot have less than 1 philosopher\n", ARGUMENT_ERROR);
	return SUCCESS;
}

int init_philo(int ac, char**av, t_philo_simu** simulation)
{
	int				i;
	int				err;
	t_philo_simu	*simu;

	if (ac < 5 || ac > 6 )
        return error_msg("Bad number of arguments\n help:\t./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n\n", ARGUMENT_ERROR);
	if (( simu = (t_philo_simu*) malloc(sizeof(t_philo_simu))) == NULL
	||	( simu->philos = (pthread_t*) malloc( sizeof(pthread_t) * simu->number_of_philosopher )) == NULL
	||	( simu->forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * simu->nb_fork)) == NULL)
		return error_msg("A memory error happen when malloc\n", MEMORY_ERROR);
	i = -1;
	simu->running = TRUE;
    simu->number_of_philosopher = ft_atoi(av[1]);
    simu->time_to_die = ft_atoi(av[2]);
	simu->time_to_eat = ft_atoi(av[3]);
	simu->time_to_sleep = ft_atoi(av[4]);
	if ((err = check_param_validity(simu)))
		return err;
	( ac == 6 ) ? (simu->max_eating = ft_atoi(av[5])) : ( simu->max_eating = -1);
	simu->nb_fork = (simu->number_of_philosopher == 1) ? 2 : simu->number_of_philosopher;
	while (++i < simu->nb_fork)
		pthread_mutex_init(&(simu->forks[i]), NULL);
	pthread_mutex_init(&(simu->simu_m), NULL);
	start_game_info(simu);
	*simulation = simu;
    return SUCCESS;
}