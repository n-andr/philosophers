/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:27 by nandreev          #+#    #+#             */
/*   Updated: 2024/09/19 00:02:33 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* # Number of philosophers and forks
NUM_PHILOSOPHERS = 5

# Create a list to represent forks, each initially available
forks = [available for _ in range(NUM_PHILOSOPHERS)]

# Function for a philosopher to pick up forks
function pick_up_forks(philosopher_id):
    left_fork = philosopher_id
    right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS

    # Ensure mutual exclusion
    acquire_lock()

    # Pick up left fork
    while forks[left_fork] == not available:
        wait()
    forks[left_fork] = not available

    # Pick up right fork
    while forks[right_fork] == not available:
        wait()
    forks[right_fork] = not available

    release_lock()

# Function for a philosopher to put down forks
function put_down_forks(philosopher_id):
    left_fork = philosopher_id
    right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS

    # Ensure mutual exclusion
    acquire_lock()

    # Put down left fork
    forks[left_fork] = available

    # Put down right fork
    forks[right_fork] = available

    release_lock()

# Function representing philosopher's behavior
function philosopher(philosopher_id):
    while true:
        think(philosopher_id)
        pick_up_forks(philosopher_id)
        eat(philosopher_id)
        put_down_forks(philosopher_id)

# Initialize philosophers
for i from 0 to NUM_PHILOSOPHERS - 1:
    start_thread(philosopher, i)
*/
#include "philo.h"

int	create_philo_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philo)
	{
		sim->philosopher[i].id = i + 1;
		sim->philosopher[i].times_eaten = 0;
		sim->philosopher[i].initiation_time = get_time();
		sim->philosopher[i].last_meal_time = get_time() - sim->philosopher[i].initiation_time;
		sim->philosopher[i].sim = sim;
		sim->philosopher[i].left_fork = sim->forks[i];
		sim->philosopher[i].right_fork = sim->forks[(i + 1) % sim->num_philo];
		// if (i + 1 == sim->num_philo)
		// {
		// 	sim->philosopher[i].right_fork = sim->forks[0];
		// }
		// else
		// {
		// 	sim->philosopher[i].right_fork = sim->forks[i + 1];
		// }
		write_status("created", &sim->philosopher[i]); //delete
		pthread_create(&sim->philosopher[i].thread, NULL, routine, &sim->philosopher[i]); //check if == 0
		i++;
	}
	return (1);
}

int	init_simulation(t_simulation *sim, int argc, char **argv)
{
	int	i;

	// sim = (t_simulation *)malloc(sizeof(t_simulation));
	// if (!sim)
	// {
	// 	printf("Error: malloc failed\n");
	// 	return (0);
	// }

	sim->num_philo = ft_atoi(argv[1]);
	sim->time_to_die = ft_atoi(argv[2]);
	sim->time_to_eat = ft_atoi(argv[3]);
	sim->time_to_sleep = ft_atoi(argv[4]);
	// sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philo);
	// if (!sim->forks)
	// {
	// 	printf("Error: malloc failed\n");
	// 	return (0);
	// }
	// sim->philosopher = malloc(sizeof(t_philosopher) * sim->num_philo);
	// if (!sim->philosopher)
	// {
	// 	free(sim->forks);
	// 	printf("Error: malloc failed\n");
	// 	return (0);
	// }
	// create_philo_threads(sim);
	// //initialize forks??
	// // add error handling for atoi, if letters etc
	if (argc == 6)
		sim->must_eat_count = ft_atoi(argv[5]);
	else
		sim->must_eat_count = -1;
	i = 0;
    while (i < sim->num_philo)
    {
        pthread_mutex_init(&sim->forks[i], NULL);
        i++;
    }
    //pthread_mutex_init(&sim->print_lock, NULL);
	return (1);
}

int arg_check(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of arguments\n");
		return (0);
	}
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error: invalid argument\n");
				return (0);
			}
			j++;
		}
		if (ft_atoi(argv[i]) < 1) // check if every arg should be > 0 or just the first one
		{
			printf("Error: invalid argument\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_simulation	*sim; //maybe not a pointer

	sim = NULL;
	if (arg_check(argc, argv) == 0)
		return (0);
	init_simulation(sim, argc, argv);
	free_struct(sim);
	
	
	return (0);
}
