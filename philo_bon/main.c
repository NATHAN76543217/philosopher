#include "philo_bon.h"

/*
** Free simulation structure
*/
static int	clear_all(t_philo_simu *simu)
{
	int i;

	i = -1;
	if (simu->philos_id)
		free(simu->philos_id);
	if (sem_unlink("forks") != SUCCESS
	||	sem_unlink("eat_enough") != SUCCESS 
	|| sem_unlink("stop_simu") != SUCCESS)
		return error_msg("semaphore destruction failed\n", SYS_ERROR);
	log_simu("Simulation cleared.", simu);
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
		if (waitpid(simu->philos_id[i], NULL, 0) != simu->philos_id[i])
			return error_msg("Error when joining two process.\n", SYS_ERROR);
		printf("%ld %3d  process joined\n", elapsedStart(simu->timestamp), i );
	}
	return clear_all(simu);
}

// /*
// ** Wait for the simulation is ending before cleaning context
// */
// static int	wait_simulation_end(t_philo_simu *simu)
// {
// 	int		i;
// 	int		err;
// 	t_philo	*philo;

// 	i = -1;
	
// 	if ((err = pthread_join(simu->monitor, NULL)) != SUCCESS)
// 		return (error_msg("Error when joining monitor thread.\n", SYS_ERROR));
// 	log_simu("monitor joined", simu);
// 	while (++i < simu->number_of_philosopher)
// 	{
// 		if ((err = pthread_join(simu->threads[i], (void * )&philo)) != SUCCESS)
// 			return (error_msg("Error when joining two threads.\n", SYS_ERROR));
// 		log_philo("thread joined", philo);
// 	}
// 	destroy_all_philosophers(simu);
// 	log_simu("structure destroyed", simu);
// 	return clear_all(simu);
// }

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
	log_simu("all the philosophers are launched.", simu);
	return (SUCCESS);
}

/*
** Program's entrypoint
*/
//TODO check if structure can be inline initialised with norminette
//TODO ask for removing should stop message
//TODO secure every semaphore call
//TODO change makefile with all files explicitly named
int			main(int ac, char **av)
{
	int				err;
	t_philo_simu	simu;

	if (( err = init_simu(ac, av, &simu)) != SUCCESS
	||	( err = start_simulation(&simu)) != SUCCESS
	||	( err = wait_simulation_end(&simu)) != SUCCESS )
		return 	printf("Program exit with return code: %d\n", err);
	printf("Out of program\n");
	return (SUCCESS);
}