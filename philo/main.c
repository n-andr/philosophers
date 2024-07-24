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

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		//think(philo->id);
		//pick_up_forks(philo->id);
		//eat(philo->id);
		//put_down_forks(philo->id);
	}
	return (NULL);
}

int	create_threads(t_simulation *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		data->philosopher[i].id = i;
		data->philosopher[i].times_eaten = 0;
		data->philosopher[i].last_meal_time = get_time();
		data->philosopher[i].left_fork = i; //tbd
		data->philosopher[i].right_fork = (i + 1) % data->num_philo; //tbd
		pthread_create(&data->philosopher[i].thread, NULL, routine, &data->philosopher[i]); //check if == 0
		i++;
	}
	return (1);
}

int	init_simulation(t_simulation *data, int argc, char **argv)
{
	int	i;

	data->num_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philo);
	if (!data->forks)
	{
		printf("Error: malloc failed\n");
		return (0);
	}
	data->philosopher = malloc(sizeof(t_philosopher) * data->num_philo);
	if (!data->philosopher)
	{
		free(data->forks);
		printf("Error: malloc failed\n");
		return (0);
	}
	//initialize forks??
	// add error handling for atoi, if letters etc
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	i = 0;
    while (i < data->num_philo)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        i++;
    }
    //pthread_mutex_init(&data->print_lock, NULL);
	return (1);
}

int	main(int argc, char **argv)
{
	t_simulation	*data; //maybe not a pointer

	if (argc == 5 || argc == 6)
	{
		data = (t_simulation *)malloc(sizeof(t_simulation));
		if (!data)
		{
			printf("Error: malloc failed\n");
			return (0);
		}
		if (init_simulation(data, argc, argv) == 0)
		{
			free(data);
			//free_struct(data);
			return (0);
		}	
	}
	else
	{
		printf("Error: wrong number of arguments\n");
	}
	free_struct(data);
	return (0);
}
