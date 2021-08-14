#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>
#include <semaphore.h>

//TODO remove
#include <errno.h>

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
# define CHECK_DELAY 4

# define FALSE 0
# define TRUE 1

# define THINKING 0
# define EATING 1
# define SLEEPING 2

typedef struct		s_philo t_philo;

typedef struct		s_philo_simu
{
	int				number_of_philosopher;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_eating;
	int				nb_fork_init;
	int				count_philo_eat_enough;
	struct timeval	timestamp;
	pid_t			*philos_id;
	sem_t			*forks;
	sem_t			*writing;
	sem_t			*eat_enough;
	sem_t			*stop_simu;
}					t_philo_simu;

typedef struct		s_philo
{
	int				id;
	int				alive;
	int				eat_count;
	struct timeval	timestamp;
	struct timeval	last_meal;
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
** init_simu.c
*/

int		init_simu(int ac, char **av, t_philo_simu * simu);

/*
** listener.c
*/

int	start_end_simu_listener(t_philo *philo);
int	start_eat_enough_listener(t_philo_simu *simu);

/*
** log.c
*/

void	log_philo(char *str, const t_philo *philo);
void	log_simu(char *str, const t_philo_simu *simu);
int		error_msg(const char *str, int ret_value);

/*
** monitor.c
*/
int		start_monitoring(t_philo *philo);

/*
** philosopher.c
*/

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