#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_simulation t_simulation;
typedef struct	s_philosopher
{
	int		id;
	int	times_eaten;
    long last_meal_time;
	t_simulation *sim;
	pthread_mutex_t left_fork;
	pthread_mutex_t right_fork;
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


//routine
void	*routine(void *arg);
//utils
void	write_message(char *str, t_philosopher *philo);
int	ft_atoi(const char *str);
long	get_time(void);

//free
void	free_struct(t_simulation *data);

#endif