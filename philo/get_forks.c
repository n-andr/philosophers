/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_forks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:18:06 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/23 16:20:43 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// fork_status = 0 - available
// fork_status = 1 - busy

int	left_fork(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (philo->sim->fork_status[philo->id] == 0)
	{
		philo->sim->fork_status[philo->id] = 1;
		write_status("has taken a fork", philo);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	pthread_mutex_unlock(philo->left_fork);
	return (0);
}

int	right_fork(t_philosopher *philo)
{
	int	right_fork;

	right_fork = (philo->id + 1) % philo->sim->num_philo;
	pthread_mutex_lock(philo->right_fork);
	if (philo->sim->fork_status[right_fork] == 0)
	{
		philo->sim->fork_status[right_fork] = 1;
		write_status("has taken a fork", philo);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

int	get_forks(t_philosopher *philo)
{
	int	got_left_fork;
	int	got_right_fork;

	got_left_fork = 0;
	got_right_fork = 0;
	while (dead_check(philo) != 1)
	{
		if (philo->id % 2 == 0)
		{
			if (got_left_fork == 0)
				got_left_fork = left_fork(philo);
			if (got_right_fork == 0)
				got_right_fork = right_fork(philo);
		}
		else
		{
			if (got_right_fork == 0)
				got_right_fork = right_fork(philo);
			if (got_left_fork == 0)
				got_left_fork = left_fork(philo);
		}
		if (got_left_fork == 1 && got_right_fork == 1)
			return (0);
	}
	return (1);
}
