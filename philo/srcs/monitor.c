#include "philo.h"

// static void should_take(t_philo_simu *simu)
// {
// 	int i;

// 	i = 0;
// 	while (i < simu->number_of_philosopher)
// 	{
// 		printf("philo %d should take forks : [%d] [%d]\n", simu->philos[i]->id, simu->philos[i]->right_fork_id, simu->philos[i]->left_fork_id);
// 		i++;
// 	}
// }

static void	*monitor(void *simulation)
{
	int i;
	t_philo_simu *simu;
	
	i = 0;
	simu = (t_philo_simu *) simulation; 
	// usleep(1000);
	// should_take(simu);
	while (simu->running)
	{
		usleep(1000);
		pthread_mutex_lock(&(simu->philos[i]->philo_m));
		// check if nb eating global == nb_phlosopher
		// Check for every philosopher if they are alive and if they are starving
		if (elapsedLastMeal(simu->philos[i]) > simu->time_to_die)
		{
			simu->philos[i]->alive = FALSE;
			// simu->running = FALSE;
			printf("%ld %3d  must die cause doesn't have eat since %ld ms\n", elapsedStart(*(simu->philos[i]->timestamp)),  simu->philos[i]->id, elapsedLastMeal(simu->philos[i]));
			pthread_mutex_unlock(&(simu->philos[i]->philo_m));
			break ;
		
		}
		pthread_mutex_unlock(&(simu->philos[i]->philo_m));
		if ( simu->count_philo_eat_enought == simu->number_of_philosopher)
		{
			pthread_mutex_lock(&(simu->simu_m));
			simu->running = FALSE;
			pthread_mutex_unlock(&(simu->simu_m));
			log_simu("every philosopher have eat enought", simu);
		}
		i = (i + 1) % simu->number_of_philosopher;
	}

	return (NULL);
}

int			create_monitor(t_philo_simu *simu)
{
	int err;

	if ((err = pthread_create(&(simu->monitor), NULL, &monitor, (void *)simu)) != 0)
	   printf("\ncan't create monitor thread :[%d]", err);
	return (SUCCESS);
}