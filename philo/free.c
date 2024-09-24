/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:54 by nandreev          #+#    #+#             */
/*   Updated: 2024/09/24 13:43:20 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_all(t_simulation *sim)
{
	
	int	i;

	i = 0;
	while (i < sim->num_philo)
	{
		// if (sim->fork_status[i] == 1)
		// 	{
		// 		pthread_mutex_unlock(&sim->forks[i]);
		// 		sim->fork_status[i] = 0;
		// 	}
		pthread_mutex_destroy(&sim->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&sim->print_lock);
	free(sim->forks);
	free(sim->fork_status);
	free(sim->thread);
}