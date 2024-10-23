/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:22:51 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/23 16:41:32 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	dead_check(t_philosopher *philo)
{
	eaten_enough(philo);
	pthread_mutex_lock(&philo->sim->dead_check_lock);
	if (philo->sim->is_dead == 1)
	{
		pthread_mutex_unlock(&philo->sim->dead_check_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->sim->dead_check_lock);
	philo->time_passed = 
		get_time() - philo->initiation_time - philo->last_meal_time;
	if (philo->time_passed >= philo->sim->time_to_die)
	{
		write_status("died", philo);
		pthread_mutex_lock(&philo->sim->dead_check_lock);
		philo->sim->is_dead = 1;
		pthread_mutex_unlock(&philo->sim->dead_check_lock);
		return (1); 
	}
	return (0);
}
