/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:54 by nandreev          #+#    #+#             */
/*   Updated: 2024/09/22 16:45:24 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_all(t_simulation *sim)
{
	
	int	i;

	i = 0;
	while (i < sim->num_philo)
	{
		if (sim->fork_status[i] == 1)
			pthread_mutex_unlock(&sim->forks[i]);
		pthread_mutex_destroy(&sim->forks[i]);
		i++;
	}
	free(sim->forks);
	free(sim->fork_status);
	free(sim->thread);
	pthread_mutex_destroy(&sim->print_lock);
}