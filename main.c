#include "philo.h"

int	wait_simulation_end(t_philo_simu *simu)
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
	return SUCCESS;
}

int start_simulation(t_philo_simu *simu)
{
	int i;
	int	err;

	i = -1;
	while(++i < simu->number_of_philosopher)
	{
		if ((err = create_philosopher(simu, i)) != SUCCESS)
			return err;
	}
	printf("Simuation started\n");
	return SUCCESS;
}

int main(int ac, char **av)
{
	int				err;
	t_philo_simu	*simu;

	simu = NULL;
	if (( err = init_simu(ac, av, &simu)) != SUCCESS
	||	( err = start_simulation(simu)) != SUCCESS
	||	( err = wait_simulation_end(simu)) != SUCCESS
	||	( err = clear_all( simu )) != SUCCESS )
		return 	printf("Program return %d\n", err);
	printf("Out of program\n");
	simu = NULL;
	usleep(100000000);
	return SUCCESS;
}
// TODO add timestamp