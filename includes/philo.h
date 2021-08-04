#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

# define SYS_ERROR -2
# define MEMORY_ERROR -1
# define SUCCESS 0
# define ARGUMENT_ERROR 1
# define END_SIMULATION 2
# define FALSE 0
# define TRUE 1

enum fork_state {
	FREE,
	USED
};

enum activities {
	SLEEPING,
	THINKING,
	EATING
};

typedef struct s_philo_simu
{
	int number_of_philosopher;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int max_eating;
	int running;
	pthread_mutex_t *forks;
	pthread_mutex_t simu_m; 
	int nb_fork;
	pthread_t *philos;

}		t_philo_simu;

typedef struct s_philo
{
	int		id;
	int		right_fork_id;
	int		left_fork_id;
	int		eat_count;
	// int		activity;
	struct timeval	*timestamp;
	struct timeval	*last_meal;
	struct timeval	*start_activity;
	t_philo_simu *simu;
}		t_philo;


int		init_philo(int ac, char** av, t_philo_simu** mPhilo);
void	create_philosopher(t_philo_simu* simu, int i);
void	dump_game_info(t_philo_simu* mPhilo);
long	elapsedLastMeal(t_philo *philo);
long	elapsedStart(t_philo *philo);
int		clear_all(t_philo_simu *simu);
int		ft_atoi(const char *str);
int		error_msg(const char *str, int ret_value);



#endif		//PHILO_H