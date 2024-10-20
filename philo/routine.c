/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:32 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/21 18:10:57 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void think(t_philosopher *philo)
{
	int time_to_think;
	//philo->time_passed = get_time() - philo->initiation_time - philo->last_meal_time;

	pthread_mutex_lock(&philo->sim->print_lock);
	write_status("is thinking", philo);
	pthread_mutex_unlock(&philo->sim->print_lock);
	time_to_think = philo->sim->time_to_die - philo->sim->time_to_eat - philo->sim->time_to_sleep;
	// while (philo->sim->time_to_die > get_time() - (philo->last_meal_time - philo->initiation_time - philo->sim->time_to_eat)
	// 	&& dead_check(philo) != 1)
	// {
	// 	usleep(100);
	// }

	if (time_to_think > 0)
	{
		usleep(1000);
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
	pthread_mutex_lock(&philo->sim->print_lock);
	eaten_enough(philo);
	pthread_mutex_lock(&philo->sim->dead_check_lock);
	if (philo->sim->is_dead == 1)
	{
		pthread_mutex_unlock(&philo->sim->dead_check_lock);
		pthread_mutex_unlock(&philo->sim->print_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->sim->dead_check_lock);

	philo->time_passed = get_time() - philo->initiation_time - philo->last_meal_time;
	if (philo->time_passed >= philo->sim->time_to_die)
	{
		write_status("died", philo);
		pthread_mutex_unlock(&philo->sim->print_lock);
		pthread_mutex_lock(&philo->sim->dead_check_lock);
		philo->sim->is_dead = 1;
		pthread_mutex_unlock(&philo->sim->dead_check_lock);

		return (1); 
	}
	pthread_mutex_unlock(&philo->sim->print_lock);
	return (0);
}

// fork_status = 0 - available
// fork_status = 1 - busy

int left_fork(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	if (philo->sim->fork_status[philo->id] == 0)
	{
		philo->sim->fork_status[philo->id] = 1;
		
		pthread_mutex_lock(&philo->sim->print_lock);
		//printf("philo[i] = %d, philo->left_fork %d\n",philo->id, philo->id); //delete
		write_status("has taken a left fork", philo);
		pthread_mutex_unlock(&philo->sim->print_lock);
		pthread_mutex_unlock(&philo->left_fork);
		return (1);
	}
	pthread_mutex_unlock(&philo->left_fork);
	return (0);
}

int right_fork(t_philosopher *philo)
{
	int right_fork;

	right_fork = (philo->id + 1) % philo->sim->num_philo;
	pthread_mutex_lock(&philo->right_fork);
	if (philo->sim->fork_status[right_fork] == 0)
	{
		philo->sim->fork_status[right_fork] = 1;
		pthread_mutex_lock(&philo->sim->print_lock);
		//printf("philo[i] = %d, philo->right_fork %d\n", philo->id, right_fork); //delete
		write_status("has taken a right fork", philo);
		pthread_mutex_unlock(&philo->sim->print_lock);
		pthread_mutex_unlock(&philo->right_fork);
		return (1);
	}
	pthread_mutex_unlock(&philo->right_fork);
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
		if (got_left_fork == 1 && got_right_fork == 1)
			return (0);
	}
	return (1);
}

int sleep_good(t_philosopher *philo, int duration, char *msg)
{
	size_t start_sleeping;

	if (dead_check(philo) == 1)
		return (1);
	start_sleeping = get_time();
	pthread_mutex_lock(&philo->sim->print_lock);
    write_status(msg, philo);
	pthread_mutex_unlock(&philo->sim->print_lock);

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
    //change usleep to sleep_good to check if philo is dead
	sleep_good(philo, philo->sim->time_to_eat, "is eating");
	pthread_mutex_lock(&philo->left_fork);
	philo->sim->fork_status[philo->id] = 0;
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_lock(&philo->right_fork);
	philo->sim->fork_status[(philo->id + 1) % philo->sim->num_philo] = 0;
	pthread_mutex_unlock(&philo->right_fork);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 0) // Simulate a delay for even philosophers
		usleep(500); // maybe use sleep_good() 
	while (dead_check(philo) != 1)
	{
		// if (pick_up_left_fork(philo) == 1 
		// 	|| pick_up_right_fork(philo) == 1)
		// 	return (NULL);
		if (get_forks(philo) == 1)
			return (NULL);
		eat(philo);
		if (sleep_good(philo, philo->sim->time_to_sleep, "is sleeping") == 1)
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