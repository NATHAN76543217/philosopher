#include "philo_bonus.h"

/*
** Free simulation structure
*/
static int	clear_all(t_philo_simu *simu)
{
	if (!simu)
		return error_msg("func: clear_all simu == NULL\n", SYS_ERROR);
	if (simu->philos)
		free(simu->philos);
	if (sem_unlink("forks") != SUCCESS
	||	sem_unlink("simu_m") != SUCCESS )
			return error_msg("semaphore destruction failed\n", SYS_ERROR);
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

	i = -1;
	while (++i < simu->number_of_philosopher)
	{
		//TODO check return value
		if (waitpid(simu->philos[i], NULL, 0) != simu->philos[i])
			return error_msg("Error when joining two process.\n", SYS_ERROR);
		#ifdef DEBUG
			printf("process %d joined\n", i);
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

	if (( err = init_simu(ac, av, &simu)) != SUCCESS
	||	( err = start_simulation(&simu)) != SUCCESS
	||	( err = wait_simulation_end(&simu)) != SUCCESS )
		return 	printf("Program exit with return code: %d\n", err);
	printf("Out of program\n");
	return SUCCESS;
}