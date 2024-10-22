/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:32 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/22 23:36:54 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	usleep(100) - 0.1ms
//	usleep(1000) - 1ms
//	usleep(10000) - 10ms
//	usleep(microseconds) 
//  1 microsecond = 0.001 milliseconds
//  1 millisecond = 1000 microseconds

void think(t_philosopher *philo)
{
	int time_to_think;
	//philo->time_passed = get_time() - philo->initiation_time - philo->last_meal_time;

	// pthread_mutex_lock(&philo->sim->print_lock);
	write_status("is thinking", philo);
	// pthread_mutex_unlock(&philo->sim->print_lock);
	time_to_think = (philo->sim->time_to_die - philo->sim->time_to_eat - philo->sim->time_to_sleep) / 2;
	// while (philo->sim->time_to_die > get_time() - (philo->last_meal_time - philo->initiation_time - philo->sim->time_to_eat)
	// 	&& dead_check(philo) != 1)
	// {
	// 	usleep(100);
	// }

	if (time_to_think > 0 && time_to_think < 1000)
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
	if (philo->sim->must_eat != -1 && philo->times_eaten >= philo->sim->must_eat)
	{
		philo->sim->ate_enouth++;
		philo->times_eaten = -1;
	}
	if (philo->sim->must_eat != -1 
		&& philo->sim->ate_enouth == philo->sim->num_philo)
	{
		pthread_mutex_lock(&philo->sim->dead_check_lock);
		philo->sim->is_dead = 1;
		pthread_mutex_unlock(&philo->sim->dead_check_lock);
		return (1);
	}
	return (0);
}

int dead_check(t_philosopher *philo)
{
	// pthread_mutex_lock(&philo->sim->print_lock);
	eaten_enough(philo);
	pthread_mutex_lock(&philo->sim->dead_check_lock);
	if (philo->sim->is_dead == 1)
	{
		pthread_mutex_unlock(&philo->sim->dead_check_lock);
		// pthread_mutex_unlock(&philo->sim->print_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->sim->dead_check_lock);

	philo->time_passed = get_time() - philo->initiation_time - philo->last_meal_time;
	if (philo->time_passed >= philo->sim->time_to_die)
	{
		write_status("died", philo);
		// pthread_mutex_unlock(&philo->sim->print_lock);
		pthread_mutex_lock(&philo->sim->dead_check_lock);
		philo->sim->is_dead = 1;
		pthread_mutex_unlock(&philo->sim->dead_check_lock);
		return (1); 
	}
	// pthread_mutex_unlock(&philo->sim->print_lock);
	return (0);
}

// fork_status = 0 - available
// fork_status = 1 - busy

int left_fork(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);

	if (philo->sim->fork_status[philo->id] == 0)
	{
		philo->sim->fork_status[philo->id] = 1;
		
		// pthread_mutex_lock(&philo->sim->print_lock);
		//printf("&philo->left_fork %d adress %p:\n", philo->id, &philo->left_fork); //del

		//printf("philo[i] = %d, philo->left_fork %d\n",philo->id, philo->id); //delete
		write_status("has taken a left fork", philo);

		// pthread_mutex_unlock(&philo->sim->print_lock);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	pthread_mutex_unlock(philo->left_fork);
	return (0);
}

int right_fork(t_philosopher *philo)
{
	int right_fork;

	right_fork = (philo->id + 1) % philo->sim->num_philo;
	pthread_mutex_lock(philo->right_fork);

	if (philo->sim->fork_status[right_fork] == 0)
	{
		philo->sim->fork_status[right_fork] = 1;
		// pthread_mutex_lock(&philo->sim->print_lock);
		//printf("&philo->right_fork %d adress %p:\n", right_fork, &philo->right_fork); //del

		//printf("philo[i] = %d, philo->right_fork %d\n", philo->id, right_fork); //delete
		write_status("has taken a right fork", philo);
		// pthread_mutex_unlock(&philo->sim->print_lock);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

int get_forks(t_philosopher *philo)
{
	int got_left_fork;
	int got_right_fork;
	// int right_fork;

	// right_fork = (philo->id + 1) % philo->sim->num_philo;
	got_left_fork = 0;
	got_right_fork = 0;
	write_status("get forks start", philo); //DEBUG
	while (dead_check(philo) != 1)
	{
		if (philo->id % 2 == 0)
		{
			if (got_left_fork == 0)
			{
				got_left_fork = left_fork(philo);
			}
			if (got_right_fork == 0)
			{
				got_right_fork = right_fork(philo);
			}
		}
		else
		{
			if (got_right_fork == 0)
			{
				got_right_fork = right_fork(philo);
			}
			if (got_left_fork == 0)
			{
				got_left_fork = left_fork(philo);
			}
		}
		if (got_left_fork == 1 && got_right_fork == 1) {
			write_status("got both forks", philo); //DEBUG
			return (0);
		}
	}
	write_status("get forks: dead", philo); //DEBUG
	return (1);
}

int occupied_with(t_philosopher *philo, int duration, char *msg)
{
	size_t start_sleeping;

	if (dead_check(philo) == 1)
		return (1);
	start_sleeping = get_time();
	// pthread_mutex_lock(&philo->sim->print_lock);
    write_status(msg, philo);
	// pthread_mutex_unlock(&philo->sim->print_lock);

	while (get_time() - start_sleeping < (size_t)duration)
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
    //change usleep to occupied_with to check if philo is dead
	occupied_with(philo, philo->sim->time_to_eat, "is eating");
	pthread_mutex_lock(philo->left_fork);
	philo->sim->fork_status[philo->id] = 0;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	philo->sim->fork_status[(philo->id + 1) % philo->sim->num_philo] = 0;
	pthread_mutex_unlock(philo->right_fork);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 0) // Simulate a delay for even philosophers
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