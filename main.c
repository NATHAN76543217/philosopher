#include "philo.h"

int	wait_simulation_end(t_philo_simu *simu)
{
	int i;

	i = 0;
	while (i < simu->number_of_philosopher)
	{
		if (pthread_join(simu->philos[i], NULL))
			return SYS_ERROR;
		printf("joined %d\n", i);
		i++;
	}
	return SUCCESS;
}

int start_simulation(t_philo_simu *simu)
{
	int i = 0;

	while(i < simu->number_of_philosopher)
	{
		create_philosopher(simu, i);
		i++;
	}
	usleep(10000);
	return SUCCESS;
}

void eat()
{

	// release fork
}
int main(int ac, char **av)
{
	t_philo_simu *simu = NULL;
	int err;
	// pthread_mutex_t lock;

	// pthread_mutex_init(&lock, NULL);
	if ((err = init_philo(ac, av, &simu)) != SUCCESS)
		return err;
	if ((err = start_simulation(simu)) != SUCCESS)
		return err;
	// dprintf(1, "Simulation start done.\n");
	if ((err = wait_simulation_end(simu)) != SUCCESS)
		return err;
	dprintf(1, "Simulation done.\n");
	clear_all(simu);
	dprintf(1, "Simulation cleared.\n");

	return SUCCESS;
}
// TODO add timestamp