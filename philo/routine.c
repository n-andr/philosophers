/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:32 by nandreev          #+#    #+#             */
/*   Updated: 2024/09/24 22:34:25 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void think(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->sim->print_lock);
	write_status("is thinking", philo);
	pthread_mutex_unlock(&philo->sim->print_lock);
}

// 1 - dead
// 0 - alive
int	eaten_enough(t_philosopher *philo)
{
	if (philo->sim->must_eat != -1 && philo->times_eaten >= philo->sim->must_eat)
	{
		philo->sim->ate_enouth++;
		philo->times_eaten = -1;
	}
	if (philo->sim->must_eat != -1 
		&& philo->sim->ate_enouth == philo->sim->num_philo)
	{
		philo->sim->is_dead = 1;
		return (1);
	}
	return (0);
}

int dead_check(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->sim->print_lock);
	eaten_enough(philo);
	if (philo->sim->is_dead == 1)
	{
		pthread_mutex_unlock(&philo->sim->print_lock);
		return (1);
	}
	philo->time_passed = get_time() - philo->initiation_time - philo->last_meal_time;
	if (philo->time_passed >= philo->sim->time_to_die)
	{
		write_status("died", philo);
		pthread_mutex_unlock(&philo->sim->print_lock);
		philo->sim->is_dead = 1;
		return (1); 
	}
	pthread_mutex_unlock(&philo->sim->print_lock);
	return (0);
}

// fork_status = 0 - available
// fork_status = 1 - busy

int pick_up_left_fork(t_philosopher *philo)
{
	while (dead_check(philo) != 1)
	{
		if (philo->sim->fork_status[philo->id] == 0)
		{
			philo->sim->fork_status[philo->id] = 1;
			pthread_mutex_lock(&philo->left_fork);
			pthread_mutex_lock(&philo->sim->print_lock);
			write_status("has taken a fork", philo);
			pthread_mutex_unlock(&philo->sim->print_lock);
			return (0);
		}
		//usleep(100);
	}
	return (1);
}

int pick_up_right_fork(t_philosopher *philo)
{
	int right_fork;

	right_fork = (philo->id + 1) % philo->sim->num_philo;
	while (dead_check(philo) != 1)
	{
		if (philo->sim->fork_status[right_fork] == 0)
		{
			philo->sim->fork_status[right_fork] = 1;
			pthread_mutex_lock(&philo->right_fork);	
			pthread_mutex_lock(&philo->sim->print_lock);	
			write_status("has taken a fork", philo);
			write_status("philo->right_fork %d", philo->id); //delete
			pthread_mutex_unlock(&philo->sim->print_lock);
			return (0);
		}
		//usleep(100);
	}
	pthread_mutex_unlock(&philo->left_fork);
	philo->sim->fork_status[philo->id] = 0;
	return (1);
}

int sleep_good(t_philosopher *philo)
{
	size_t start_sleeping;

	start_sleeping = get_time();
	pthread_mutex_lock(&philo->sim->print_lock);
    write_status("is sleeping", philo);
	pthread_mutex_unlock(&philo->sim->print_lock);

	while (get_time() - start_sleeping < (size_t)philo->sim->time_to_sleep)
	{
		if (dead_check(philo) == 1)
			return (1);
		usleep(100); //maybe lower this value
	}
	return (0);
}

void eat(t_philosopher *philo)
{
    philo->last_meal_time = get_time() - philo->initiation_time;  // Update last meal time
    if (philo->times_eaten != -1)
		philo->times_eaten++;
	pthread_mutex_lock(&philo->sim->print_lock); // maybe move to routine
    write_status("is eating", philo);
	pthread_mutex_unlock(&philo->sim->print_lock); // maybe move to routine
    usleep(philo->sim->time_to_eat * 1000);  // Simulate eating
	//philo->last_meal_time = get_time();  // Update last meal time
	pthread_mutex_unlock(&philo->left_fork);
	philo->sim->fork_status[philo->id] = 0;
	pthread_mutex_unlock(&philo->right_fork);
	philo->sim->fork_status[(philo->id + 1) % philo->sim->num_philo] = 0;
}

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 0) // Simulate a delay for even philosophers
		usleep(100); // maybe use sleep_good() 
	while (philo->sim->is_dead == 0)
	{
		if (pick_up_left_fork(philo) == 1 
			|| pick_up_right_fork(philo) == 1)
			return (NULL);
		eat(philo);
		if (sleep_good(philo) == 1)
			return (NULL);
		think(philo);
		// if (get_time() - philo->last_meal_time > philo->sim->time_to_die)
        // {
        //    write_status("died", philo);
        //     break;
        // }
	}
	return (NULL);
}