/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:27 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/23 16:40:50 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	strart_threads(t_simulation *sim, t_philosopher *philo)
{
	int	i;

	i = 0;
	sim->thread = malloc(sizeof(pthread_t) * sim->num_philo);
	while (i < sim->num_philo)
	{
		pthread_create(&sim->thread[i], NULL, routine, &philo[i]);
		i++;
	}
}

int	create_philo(t_simulation *sim, t_philosopher *philo)
{
	int	i;

	i = 0;
	while (i < sim->num_philo)
	{
		philo[i].id = i;
		philo[i].times_eaten = 0;
		philo[i].initiation_time = get_time();
		philo[i].last_meal_time = get_time() - philo[i].initiation_time;
		philo[i].sim = sim;
		philo[i].left_fork = &sim->forks[i];
		philo[i].right_fork = &sim->forks[(i + 1) % sim->num_philo];
		i++;
	}
	return (1);
}

int	init_forks(t_simulation *sim)
{
	int	i;

	i = 0;
	sim->fork_status = malloc(sizeof(int) * sim->num_philo);
	if (!sim->fork_status)
	{
		printf("Error: malloc failed\n");
		return (1);
	}
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philo);
	if (!sim->forks)
	{
		printf("Error: malloc failed\n");
		return (1);
	}
	pthread_mutex_init(&sim->print_lock, NULL);
	pthread_mutex_init(&sim->dead_check_lock, NULL);
	pthread_mutex_init(&sim->ate_enouth_lock, NULL);
	while (i < sim->num_philo)
	{
		pthread_mutex_init(&sim->forks[i], NULL);
		sim->fork_status[i] = 0;
		i++;
	}
	return (0);
}

int	init_simulation(t_simulation *sim, int argc, char **argv)
{
	sim->num_philo = ft_atoi(argv[1]);
	sim->time_to_die = ft_atoi(argv[2]);
	sim->time_to_eat = ft_atoi(argv[3]);
	sim->time_to_sleep = ft_atoi(argv[4]);
	sim->ate_enouth = 0;
	sim->is_dead = 0;
	if (argc == 6)
		sim->must_eat = ft_atoi(argv[5]);
	else
		sim->must_eat = -1;
	return (1);
}

int	main(int argc, char **argv)
{
	t_simulation	sim; 
	t_philosopher	*philo;

	if (arg_check(argc, argv) == 0)
		return (0);
	init_simulation(&sim, argc, argv);
	if (init_forks(&sim) == 1)
		return (1);
	philo = malloc(sizeof(t_philosopher) * sim.num_philo);
	if (!philo)
	{
		printf("Error: malloc failed\n");
		return (1);
	}
	create_philo(&sim, philo);
	strart_threads(&sim, philo);
	wait_threads(&sim);
	free_all(&sim);
	free(philo);
	return (0);
}
