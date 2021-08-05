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

// # define DEBUG

typedef struct		s_philo_simu
{
	int				number_of_philosopher;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_eating;
	int				running;
	int				nb_fork;
	pthread_t		*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	simu_m; 

}					t_philo_simu;

typedef struct		s_philo
{
	int				id;
	int				right_fork_id;
	int				left_fork_id;
	int				eat_count;
	struct timeval	*timestamp;
	struct timeval	*last_meal;
	t_philo_simu	*simu;
}					t_philo;

/*
** utils.c
*/
int		ft_atoi(const char *str);
int		error_msg(const char *str, int ret_value);
long	elapsedStart(const t_philo *philo);
long	elapsedLastMeal(const t_philo *philo);

/*
** init_simu.c
*/
int		init_simu(int ac, char **av, t_philo_simu** simulation);

/*
** philosopher.c
*/
int     create_philosopher(t_philo_simu *simu, int id);
void	destroy_philosopher(t_philo *philo);

/*
** actions.c
*/
int		takeFork(const t_philo *philo);
int		philo_sleep(const t_philo *philo);
int		philo_eat(t_philo *philo);

#endif		//PHILO_H