#include "philo.h"

void think(t_philosopher *philo)
{
	write_message("is thinking", philo);
}

void pick_up_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	write_message("has taken a left fork", philo);

	pthread_mutex_lock(&philo->right_fork);
	write_message("has taken a right fork", philo);
}

void put_down_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
	write_message("is sleeping", philo);
}

void sleep_good(t_philosopher *philo)
{
    t_simulation *sim = philo->sim;

    write_message("is sleeping", philo);
    usleep(sim->time_to_sleep * 1000);  // Simulate sleeping
}

void eat(t_philosopher *philo)
{
    t_simulation *sim = philo->sim;

    philo->last_meal_time = get_time();  // Update last meal time
    philo->times_eaten++;
    write_message("is eating", philo);
    usleep(sim->time_to_eat * 1000);  // Simulate eating
	philo->last_meal_time = get_time();  // Update last meal time

}

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		think(philo);
		pick_up_forks(philo);
		//eat(philo->id);
		put_down_forks(philo);
		//sleep(philo->id);
		if (get_time() - philo->last_meal_time > philo->sim->time_to_die)
        {
           write_message("died", philo);
            break;
        }
	}
	return (NULL);
}