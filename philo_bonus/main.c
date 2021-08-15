#include "philo_bonus.h"

/*
** free and destroy every semaphores
*/
static int	destroy_semaphores(t_philo_simu *simu)
{
	sem_post(simu->forks);
	sem_post(simu->eat_enough);
	sem_post(simu->stop_simu);
	sem_post(simu->writing);
	if (sem_close(simu->forks) != SUCCESS
	||	sem_close(simu->eat_enough) != SUCCESS
	||	sem_close(simu->stop_simu) != SUCCESS
	||	sem_close(simu->writing) != SUCCESS)
		return (SYS_ERROR);
	if (sem_unlink("forks") != SUCCESS
	||	sem_unlink("eat_enough") != SUCCESS 
	||	sem_unlink("stop_simu") != SUCCESS 
	||	sem_unlink("writing") != SUCCESS)
		return (SYS_ERROR);
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
	if (stop_eat_enough_listener(simu))
		return error_msg("listener destruction failed\n", SYS_ERROR);
	if (destroy_semaphores(simu) != SUCCESS)
		return error_msg("semaphore destruction failed\n", SYS_ERROR);
	printf("%ld   -   %s\n", elapsedStart(simu->timestamp), "Simulation cleared.");	
	return (SUCCESS);
}


/*
** Wait for the end the simulation
*/
static int	wait_simulation_end(t_philo_simu *simu)
{
	int	i;

	i = -1;
	while (++i < simu->number_of_philosopher)
	{
		if (waitpid(simu->philos_id[i], NULL, 0) != simu->philos_id[i])
			return error_msg("Error when joining two process.\n", SYS_ERROR);
		simu->running = FALSE;
		printf("%ld %3d   process joined\n", elapsedStart(simu->timestamp), i + 1 );
	}
	return (SUCCESS);
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
//TODO Check for leaks
int			main(int ac, char **av)
{
	int				err;
	t_philo_simu	simu;

	if (( err = init_simulation(ac, av, &simu)) != SUCCESS
	||	( err = start_simulation(&simu)) != SUCCESS
	||	( err = wait_simulation_end(&simu)) != SUCCESS 
	||	( err = clear_simulation(&simu)) != SUCCESS)
		return 	printf("Program exit with return code: %d\n", err);
	printf("Out of program\n");
	return (SUCCESS);
}