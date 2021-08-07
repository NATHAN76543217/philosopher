#include "philo_bonus.h"

/*
** Dump simulation config
*/
static void	start_game_info(t_philo_simu const *simu)
{
	printf("\n---- philo simulation ----\n\n");
	printf("number_of_philosophers = %d\n", simu->number_of_philosopher);
	printf("time_to_die  = %d\n", simu->time_to_die);
	printf("time_to_eat = %d\n", simu->time_to_eat);
	printf("time_to_sleep = %d\n", simu->time_to_sleep);
	( simu->max_eating < 0 )
		? printf("[number_of_times_each_philosopher_must_eat] = \"(disactived)\"\n")
		: printf("[number_of_times_each_philosopher_must_eat] = %d\n", simu->max_eating);
	printf("nb_fork = %d\n", simu->nb_fork_init);
	dprintf(1, "Simulation initialisation done.\n\n------\n\n");
	return ;
}

/*
** Check if the simulation's settings are correct 
*/
static int	check_param_validity(t_philo_simu const *simu)
{
	if (simu->number_of_philosopher <= 0)
		return error_msg("You cannot have less than 1 philosopher.\n", ARGUMENT_ERROR);
	else if (simu->time_to_die <= 0)
		return error_msg("You cannot have a 'time_to_die' value less or equal than 0.\n", ARGUMENT_ERROR);
	else if (simu->time_to_eat <= 0)
		return error_msg("You cannot have a 'time_to_eat' value less or equal than 0.\n", ARGUMENT_ERROR);
	else if (simu->time_to_sleep <= 0)
		return error_msg("You cannot have a 'time_to_sleep' value less or equal than 0.\n", ARGUMENT_ERROR);
	else if (simu->max_eating < -1)
		return error_msg("You cannot have a 'number_of_times_each_philosopher_must_eat' value less than -1.\n", ARGUMENT_ERROR);
	else if (simu->nb_fork_init < 2)
		return error_msg("You cannot have a number of forks less than 2.\nThis behavior is unexpected, please report us the problem.\n", SYS_ERROR);
	return SUCCESS;
}

/*
** Initialise simulation child processes
*/
static int	semaphores_initialisation(t_philo_simu *simu)
{
	// TODO only for a time?
	sem_unlink("forks");
	sem_unlink("simu_m");
	if (( simu->forks = sem_open("forks", O_CREAT | O_EXCL, S_IRWXU, simu->nb_fork_init)) == SEM_FAILED )
		return error_msg("semaphore (forks) initialisation failed\n", SYS_ERROR);
	if (( simu->simu_m = sem_open("simu_m", O_CREAT | O_EXCL, S_IRWXU, 1)) == SEM_FAILED )
	{
		sem_close(simu->forks);
		return error_msg("semaphore (simu_m) initialisation failed\n", SYS_ERROR);
	}
	return SUCCESS;
}

/*
** Init simulation with program params
*/
int			init_simu(int ac, char **av, t_philo_simu *const simu)
{
	int				err;

	if (ac < 5 || ac > 6 )
        return error_msg("Bad number of arguments\n\n\tUsage:\t./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n\n", ARGUMENT_ERROR);
	memset(simu, 0, sizeof(t_philo_simu));
	simu->running = TRUE;
    simu->number_of_philosopher = ft_atoi(av[1]);
    simu->time_to_die = ft_atoi(av[2]);
	simu->time_to_eat = ft_atoi(av[3]);
	simu->time_to_sleep = ft_atoi(av[4]);
	simu->nb_fork_init = (simu->number_of_philosopher == 1) ? 2 : simu->number_of_philosopher;
	simu->max_eating = ( ac == 6 ) ? ft_atoi(av[5]) : ( simu->max_eating = -1);
	if (( simu->philos = (pid_t *) malloc( sizeof(pid_t) * simu->number_of_philosopher )) == NULL)
		return error_msg("A memory error happen when malloc (simu)\n", MEMORY_ERROR);
	if (( err = check_param_validity(simu) != SUCCESS )
	||	( err = semaphores_initialisation(simu)) != SUCCESS )
		return err;
	start_game_info(simu);
    return SUCCESS;
}