#include "philo.h"

/*
** Free a philosopher structure
*/
static void	destroy_philosopher(t_philo *philo)
{
	if (!philo)
		return ;
	log_philo("structure destroyed", philo);
	if ( philo->timestamp )
		free( philo->timestamp );
	if ( philo->last_meal )
		free( philo->last_meal );
	free(philo);
	return ;
}

/*
** Free all philospher structures in the simulation
*/
void		destroy_all_philosophers(t_philo_simu *simu)
{
	int i;

	i = 0;
	while (i < simu->number_of_philosopher)
	{
		destroy_philosopher(simu->philos[i]);
		i++;
	}
	return ;
}

