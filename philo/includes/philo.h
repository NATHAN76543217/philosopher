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

# define MAX_EAT 100
# define MAX_TIME 10000
# define MAX_TIME_S #MAX_TIME
# define STRE(x) #x
# define STRI(x) STRE(x)
# define MAX_PHILO 100

# define FALSE 0
# define TRUE 1

# define THINKING 0
# define EATING 1
# define SLEEPING 2

# define DEBUG

typedef struct		s_philo t_philo;

typedef struct		s_philo_simu
{
	int				number_of_philosopher;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_eating;
	int				running;
	int				nb_fork;
	int				count_philo_eat_enought;
	struct timeval	timestamp;
	t_philo			**philos;
	pthread_t		monitor;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	simu_m;

}					t_philo_simu;

typedef struct		s_philo
{
	int				id;
	int				alive;
	int				right_fork_id;
	int				left_fork_id;
	int				eat_count;
	struct timeval	*timestamp;
	struct timeval	*last_meal;
	pthread_mutex_t	philo_m;
	t_philo_simu	*simu;
	int				(*activity[3]) (t_philo *philo);
}					t_philo;


/*
** activity.c
*/

int		philo_eat(t_philo *philo);
int		philo_sleep(t_philo *philo);
int		take_forks(t_philo *philo);

/*
** destroy_philospoher.c
*/

void	destroy_all_philosophers(t_philo_simu *simu);

/*
** init_simu.c
*/

int		init_simu(int ac, char **av, t_philo_simu * simu);

/*
** log.c
*/

void	log_philo(char *str, const t_philo *philo);
void	log_simu(char *str, const t_philo_simu *simu);
int		error_msg(const char *str, int ret_value);

/*
** monitor.c
*/
int		create_monitor(t_philo_simu *simu);

/*
** philosopher.c
*/

int		shouldStopSimu(const t_philo *philo);
int     create_philosopher(t_philo_simu *simu, int id);

/*
** time.c
*/

long	elapsedStart(const struct timeval timestamp);
long	elapsedLastMeal(const t_philo *philo);

/*
** utils.c
*/

int		ft_atoi(const char *str);
void	*ft_memcpy(void *dest, const void *src, size_t n);





#endif		//PHILO_H