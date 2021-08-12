#include "philo_bon.h"


static void	*monitor(void *philosopher)
{
	int i;
	t_philo *philo;
	
	i = 0;
	philo = (t_philo *) philosopher; 
	while (philo->alive)
	{
		usleep(1000);
		if (elapsedLastMeal(philo) > philo->simu->time_to_die)
		{
			philo->alive = FALSE;
			printf("%ld %3d  must die cause doesn't have eat since %ld ms\n", elapsedStart(philo->timestamp),  philo->id, elapsedLastMeal(philo));
			break ;
		}
		i = (i + 1) % philo->simu->number_of_philosopher;
	}

	return (NULL);
}

static int	create_monitor(t_philo_simu *simu)
{
	int			err;
	pthread_t	monitor_id;

	if ((err = pthread_create(&monitor_id, NULL, &monitor, (void *)simu)) != 0)
	{
	   printf("\ncan't create monitor thread :[%d]", err);
		return (SYS_ERROR);
	}
	if (pthread_detach(monitor_id) != SUCCESS)
		return (SYS_ERROR);
	return (SUCCESS);
}

int			start_monitoring(t_philo *philo)
{
	create_monitor(philo->simu);
	start_end_simu_listener(philo);
	return SUCCESS;
}