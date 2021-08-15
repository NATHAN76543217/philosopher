#include "philo_bonus.h"


/*
** Check every CHECK_DELAY milliseconds if the philosopher is starving
*/
static void	*monitor(void *philosopher)
{
	int i;
	t_philo *philo;
	
	i = 0;
	philo = (t_philo *) philosopher;
	log_philo("monitor started.", philo);
	while (philo->alive)
	{
		usleep(CHECK_DELAY * 1000);
		if (elapsedLastMeal(philo) > philo->simu->time_to_die)
		{
			printf("%ld %3d   must die cause doesn't have eat since %ld ms\n", elapsedStart(philo->timestamp),  philo->id, elapsedLastMeal(philo));
			philo->alive = FALSE;
			sem_post(philo->simu->stop_simu);
			if (philo->current == SLEEPING)
			{
				log_philo("died.", philo);
				exit(EXIT_SUCCESS);
			}
			break ;
		}
		i = (i + 1) % philo->simu->number_of_philosopher;
	}
	log_philo("monitor stopped.", philo);
	return (NULL);
}

static int	start_monitor(t_philo *philo)
{
	int			err;

	if ((err = pthread_create(&(philo->monitor_id), NULL, &monitor, (void *)philo)) != 0)
		return log_syserror("can't create thread monitor", philo);
	return (SUCCESS);
}

int			start_monitoring(t_philo *philo)
{
	start_monitor(philo);
	start_end_simu_listener(philo);
	return SUCCESS;
}