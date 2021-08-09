#include "philo.h"

/*
** Free simulation structure
*/
static int	clear_all(t_philo_simu *simu)
{
	int i;

	i = -1;
	while (++i < simu->nb_fork)
		if (pthread_mutex_destroy(&(simu->forks[i])) != SUCCESS)
			return error_msg("mutex destruction failed\n", SYS_ERROR);
	if (simu->forks)
		free(simu->forks);
	if (simu->philos)
		free(simu->philos);
	#ifdef DEBUG
		printf("Simulation cleared.\n");
	#endif
	return SUCCESS;
}

/*
** Wait for the simulation is ending before cleaning context
*/
static int	wait_simulation_end(t_philo_simu *simu)
{
	int		i;
	int		err;
	t_philo	*philo;

	i = -1;
	while (++i < simu->number_of_philosopher)
	{
		if ((err = pthread_join(simu->philos[i], (void * )&philo)) != SUCCESS)
			return error_msg("Error when joining two threads.\n", SYS_ERROR);
		destroy_philosopher(philo);
		#ifdef DEBUG
			printf("thread %d joined\n", i);
		#endif
	}
	return clear_all(simu);
}

/*
** Start simulation with the right number of philosophers
*/
static int	start_simulation(t_philo_simu *simu)
{
	int i;
	int	err;

	i = -1;
	while(++i < simu->number_of_philosopher)
	{
		if ((err = create_philosopher(simu, i)) != SUCCESS)
			return err;
	}
	printf("Simulation started\n");
	return SUCCESS;
}

/*
** Program's entrypoint
*/
int			main(int ac, char **av)
{
	int				err;
	t_philo_simu	simu;

	//TODO make a Thread monitor
	if (( err = init_simu(ac, av, &simu)) != SUCCESS
	||	( err = start_simulation(&simu)) != SUCCESS
	||	( err = wait_simulation_end(&simu)) != SUCCESS )
		return 	printf("Program exit with return code: %d\n", err);
	printf("Out of program\n");
	return SUCCESS;
}