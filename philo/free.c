/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:54 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/23 15:56:11 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_all(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philo)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->dead_check_lock);
	pthread_mutex_destroy(&sim->ate_enouth_lock);
	free(sim->forks);
	free(sim->fork_status);
	free(sim->thread);
}
