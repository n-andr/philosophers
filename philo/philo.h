
#include <stdio.h>
#include <pthread.h>

typedef struct	s_philosopher
{
	int		id;
	int	times_eaten;
    long last_meal_time;
	pthread_t left_fork;
	pthread_t right_fork;
	pthread_t thread;
}				t_philosopher;

typedef struct	s_simulation
{
	int		num_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	//int		num_eat;
	int		must_eat_count;
	pthread_mutex_t *forks;
	//pthread_mutex_t print_lock; for writing in stdout
	t_philosopher	*philosopher;
}				t_simulation;



//utils
int	ft_atoi(const char *str);

//free
void	free_struct(t_simulation *data);