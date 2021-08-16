/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 15:04:03 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 16:10:30 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <fcntl.h>
# include <sys/time.h>
# include <semaphore.h>

# define SYS_ERROR -2
# define MEMORY_ERROR -1
# define SUCCESS 0
# define ARGUMENT_ERROR 1
# define END_SIMULATION 2

# define MAX_EAT 100
# define MAX_TIME 10000

# define MAX_PHILO 100
# define CHECK_DELAY 4

# define FALSE 0
# define TRUE 1

/*
** Defines for activities 
*/
# define THINKING 0
# define EATING 1
# define SLEEPING 2

/*
** Defines for semaphores 
*/
# define FORKS 0
# define STOP_SIMU 1
# define EAT_ENOUGH 2
# define WRITING 3

typedef struct s_philo	t_philo;

typedef struct s_philo_simu
{
	int				number_of_philosopher;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_eating;
	int				nb_fork_init;
	int				running;
	pthread_t		listener_id;
	int				count_philo_eat_enough;
	struct timeval	timestamp;
	pid_t			*philos_id;
	sem_t			*sem[4];
}					t_philo_simu;

typedef int	(*t_activity)(t_philo	*philo);

typedef struct s_philo
{
	int				id;
	int				alive;
	int				eat_count;
	int				current;
	pthread_t		monitor_id;
	struct timeval	timestamp;
	struct timeval	last_meal;
	t_philo_simu	*simu;
	t_activity		activity[3];
}				t_philo;

/*
** activity.c
*/

int		philo_eat(t_philo *philo);
int		philo_sleep(t_philo *philo);
int		take_forks(t_philo *philo);

/*
** init_philosopher.c
*/

int		create_philosopher(t_philo_simu *simu, int id);

/*
** init_simu.c
*/

int		init_simulation(int ac, char **av, t_philo_simu *simu);

/*
** listener.c
*/

int		start_end_simu_listener(t_philo *philo);
int		start_eat_enough_listener(t_philo_simu *simu);
int		stop_eat_enough_listener(t_philo_simu *simu);

/*
** log.c
*/

void	log_philo(char *str, const t_philo *philo);
void	log_simu(char *str, const t_philo_simu *simu);
int		log_syserror(char *str, const t_philo *philo);
int		error_msg(const char *str, int ret_value);

/*
** monitor.c
*/
int		start_monitoring(t_philo *philo);

/*
** philosopher.c
*/

void	philosopher(t_philo *philo);

/*
** time.c
*/

long	elapsedStart(const struct timeval timestamp);
long	elapsedLastMeal(const t_philo *philo);

/*
** utils.c
*/

int		ft_atoi(const char *str);
void	game_info(t_philo_simu const *simu);
void	*ft_memcpy(void *dest, const void *src, size_t n);

#endif		//PHILO_BONUS_H