#include "philo.h"


static int philo_eat(t_philo *philo)
{
	gettimeofday(philo->start_activity, NULL);
	printf("%ld %d is eating\n", elapsedStart(philo), philo->id);
	usleep(philo->simu->time_to_eat * 1000);
	gettimeofday(philo->last_meal, NULL);
	philo->eat_count++;
	if (philo->eat_count == philo->simu->max_eating)
	{
		printf("%ld %d has eat %d times. %d/%d\n", elapsedStart(philo), philo->id, philo->eat_count, philo->eat_count, philo->simu->max_eating);
		return END_SIMULATION;
	}	
	return SUCCESS;
}

static int philo_sleep(t_philo *philo)
{
	gettimeofday(philo->start_activity, NULL);
	printf("%ld %d is sleeping\n", elapsedStart(philo), philo->id);
	usleep(philo->simu->time_to_sleep * 1000);
	gettimeofday(philo->last_meal, NULL);
	return SUCCESS;
}

static int	takeFork(t_philo *philo)
{
	if (!pthread_mutex_lock(&(philo->simu->forks[philo->right_fork_id]) ))
	{
		printf("%ld %d has taken a fork[%d]\n", elapsedStart(philo), philo->id, philo->left_fork_id);
	}
	else
		return EXIT_FAILURE;

	if (!pthread_mutex_lock(&(philo->simu->forks[philo->left_fork_id])))
	{
		printf("%ld %d has taken a second fork[%d]\n", elapsedStart(philo), philo->id, philo->right_fork_id);
		return SUCCESS;
	}
	return EXIT_FAILURE;
}

static int	shouldStopSimu(t_philo *philo)
{
	printf("%ld %d since last meal: %ld.\n", elapsedStart(philo), philo->id, elapsedLastMeal(philo));
	if (elapsedLastMeal(philo) > philo->simu->time_to_die)
	{
		printf("%ld %d is starving.\n", elapsedStart(philo), philo->id);
		return TRUE;
	}
	return !philo->simu->running;
}

static void	*routine(void* philosopher)
{
	t_philo*	philo = (t_philo *) philosopher;

	while(TRUE)
	{
		printf("%ld %d is thinking.\n", elapsedStart(philo), philo->id);
		if (takeFork(philo) == SUCCESS)
		{
			if (shouldStopSimu(philo) || philo_eat(philo) == END_SIMULATION)
			{
				pthread_mutex_unlock(&(philo->simu->forks[philo->left_fork_id]));
				pthread_mutex_unlock(&(philo->simu->forks[philo->right_fork_id]));
				break ;
			}
			pthread_mutex_unlock(&(philo->simu->forks[philo->left_fork_id]));
			pthread_mutex_unlock(&(philo->simu->forks[philo->right_fork_id]));	
			printf("%ld %d has release two forks[%d][%d]\n", elapsedStart(philo), philo->id, philo->left_fork_id, philo->right_fork_id);
			philo_sleep(philo);
			if (shouldStopSimu(philo))
				break ;
		}
	}
	if (!philo->simu->running)
	{
		printf("End of simulation. (not running) for (%d)\n", philo->id);
		return NULL;
	}
	pthread_mutex_lock(&(philo->simu->simu_m));
	philo->simu->running = FALSE;
	if (philo->simu->max_eating != -1 && philo->eat_count < philo->simu->max_eating)
		printf("%ld %d died.  %d/%d\n", elapsedStart(philo), philo->id, philo->eat_count, philo->simu->max_eating);
	pthread_mutex_unlock(&(philo->simu->simu_m));
	printf("End of simulation. for (%d)\n", philo->id);

	return NULL;
}


// init and launch a philosopher thread
void     create_philosopher(t_philo_simu* simu, int i)
{
	int			err;
	t_philo*	philo;

	philo = (t_philo *) malloc(sizeof(t_philo));
	philo->id = i + 1;
	printf("Creating philosopher %d.\n", philo->id);
	philo->eat_count = 0;
	philo->right_fork_id = i;
	philo->left_fork_id = (i + 1) % simu->number_of_philosopher;
	philo->simu = simu;
	struct timeval *timestamp = (struct timeval*) malloc(sizeof(struct timeval));
	struct timeval *last_meal = (struct timeval*) malloc(sizeof(struct timeval));
	struct timeval *start_activity = (struct timeval*) malloc(sizeof(struct timeval));
	gettimeofday(timestamp, NULL);
	gettimeofday(last_meal, NULL);
	gettimeofday(start_activity, NULL);
	philo->timestamp = timestamp;
	philo->last_meal = last_meal;
	philo->start_activity = start_activity;
	if ((err = pthread_create(&(simu->philos[i]), NULL, &routine, (void *)philo)) != 0)
		   printf("\ncan't create thread :[%d]", err);
	// if ((err = pthread_detach(simu->philos[i])))
		// printf("\ncan't detach thread :[%d]", err);
	printf("philo created [%d]\n", philo->id);
	return ;
}