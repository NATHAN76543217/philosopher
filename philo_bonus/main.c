#include "philo_bonus.h"

/*
** free and destroy every semaphores
*/
static int	destroy_semaphores(t_philo_simu *simu)
{
	if (sem_close(simu->forks) != SUCCESS
	||	sem_close(simu->eat_enough) != SUCCESS
	||	sem_close(simu->stop_simu) != SUCCESS
	||	sem_close(simu->writing) != SUCCESS
	||	sem_unlink("forks") != SUCCESS
	||	sem_unlink("eat_enough") != SUCCESS 
	||	sem_unlink("stop_simu") != SUCCESS 
	||	sem_unlink("writing") != SUCCESS)
		return SYS_ERROR;
	return (SUCCESS);
}

/*
** Free simulation structure
*/
static int	clear_simulation(t_philo_simu *simu)
{
	int i;

	i = -1;
	if (simu->philos_id)
		free(simu->philos_id);
	if (destroy_semaphores(simu) != SUCCESS)
		return error_msg("semaphore destruction failed\n", SYS_ERROR);
	printf("%ld   -   %s\n", elapsedStart(simu->timestamp), "Simulation cleared.");	
	return (SUCCESS);
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
		if (waitpid(simu->philos_id[i], NULL, 0) != simu->philos_id[i])
			return error_msg("Error when joining two process.\n", SYS_ERROR);
		printf("%ld %3d  process joined\n", elapsedStart(simu->timestamp), i + 1 );
	}
	return clear_simulation(simu);
}

/*
** Start simulation with the right number of philosophers
*/
static int	start_simulation(t_philo_simu *simu)
{
	int i;
	int	err;

	i = -1;
	start_eat_enough_listener(simu);
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
//TODO norme project
int			main(int ac, char **av)
{
	int				err;
	t_philo_simu	simu;

	if (( err = init_simulation(ac, av, &simu)) != SUCCESS
	||	( err = start_simulation(&simu)) != SUCCESS
	||	( err = wait_simulation_end(&simu)) != SUCCESS )
		return 	printf("Program exit with return code: %d\n", err);
	printf("Out of program\n");
	return (SUCCESS);
}