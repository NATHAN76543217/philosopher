/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 13:17:20 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/17 13:33:03 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

# define SYS_ERROR -2
# define MEMORY_ERROR -1
# define SUCCESS 0
# define ARGUMENT_ERROR 1
# define END_SIMULATION 2

# define MAX_SLEEP_TIME 10000
# define MAX_EAT_TIME 10000
# define MAX_SIMU_TIME 30000
# define MAX_EAT_LIMIT 100
# define MAX_PHILO 100

# define FALSE 0
# define TRUE 1

# define THINKING 0
# define EATING 1
# define SLEEPING 2

# define DEBUG

typedef struct s_philo	t_philo;
typedef int				(*t_activity)(t_philo	*philo);

typedef struct s_philo_simu
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

typedef struct s_philo
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
	t_activity		activity[3];
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
** init_philosopher.c
*/
int		create_philosopher(t_philo_simu *simu, int id);

/*
** init_simultion.c
*/

int		init_simu(int ac, char **av, t_philo_simu *simu);

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
void	*routine(void *philosopher);

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
void	start_game_info(t_philo_simu const *simu);

#endif		//PHILO_H