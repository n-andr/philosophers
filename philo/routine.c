/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:32 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/23 16:28:22 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	usleep(100) - 0.1ms
//	usleep(1000) - 1ms
//	usleep(10000) - 10ms
//	usleep(microseconds) 
//  1 microsecond = 0.001 milliseconds
//  1 millisecond = 1000 microseconds

void	think(t_philosopher *philo)
{
	int	time_to_think;
	int	time_to_eat;
	int	time_to_sleep;
	int	time_to_die;

	time_to_eat = philo->sim->time_to_eat;
	time_to_die = philo->sim->time_to_die;
	time_to_sleep = philo->sim->time_to_sleep;
	write_status("is thinking", philo);
	time_to_think = (time_to_die - time_to_eat - time_to_sleep) / 2;
	if (time_to_think >= 10 && time_to_think < 1000)
	{
		usleep(time_to_think * 1000);
	}
	else if (time_to_think > 1000)
	{
		usleep(10000);
	}
}

// 1 - dead
// 0 - alive

int	eaten_enough(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->sim->ate_enouth_lock);
	if (philo->sim->must_eat != -1 
		&& philo->times_eaten >= philo->sim->must_eat)
	{
		philo->sim->ate_enouth++;
		philo->times_eaten = -1;
	}
	if (philo->sim->must_eat != -1 
		&& philo->sim->ate_enouth == philo->sim->num_philo)
	{
		pthread_mutex_unlock(&philo->sim->ate_enouth_lock);
		pthread_mutex_lock(&philo->sim->dead_check_lock);
		philo->sim->is_dead = 1;
		pthread_mutex_unlock(&philo->sim->dead_check_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->sim->ate_enouth_lock);
	return (0);
}

int	occupied_with(t_philosopher *philo, int duration, char *msg)
{
	size_t	start_sleeping;

	if (dead_check(philo) == 1)
		return (1);
	start_sleeping = get_time();
	write_status(msg, philo);
	while (get_time() - start_sleeping < (size_t)duration)
	{
		if (dead_check(philo) == 1)
			return (1);
		usleep(100);
	}
	return (0);
}

// last_meal_time is updated before the philosopher starts eating 
// (the reqvirement from the subject)
// using occupide_with to simulate the time the philosopher is eating
// and to check if they died while eating

void	eat(t_philosopher *philo)
{
	philo->last_meal_time = get_time() - philo->initiation_time;
	occupied_with(philo, philo->sim->time_to_eat, "is eating");
	if (philo->times_eaten != -1)
		philo->times_eaten++;
	pthread_mutex_lock(philo->left_fork);
	philo->sim->fork_status[philo->id] = 0;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	philo->sim->fork_status[(philo->id + 1) % philo->sim->num_philo] = 0;
	pthread_mutex_unlock(philo->right_fork);
}

// Simulate a delay for even philosophers
// to make sure they don't start grabing forks at the same time

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 0) 
		usleep(5000);
	while (dead_check(philo) != 1)
	{
		if (get_forks(philo) == 1)
			return (NULL);
		eat(philo);
		if (occupied_with(philo, philo->sim->time_to_sleep, "is sleeping") == 1)
			return (NULL);
		think(philo);
	}
	return (NULL);
}
