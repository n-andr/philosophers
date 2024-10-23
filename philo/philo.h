/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:49 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/23 16:34:54 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_simulation
{
	int				num_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				ate_enouth;
	int				is_dead;
	int				must_eat;
	int				*fork_status;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead_check_lock;
	pthread_mutex_t	ate_enouth_lock;
	pthread_mutex_t	print_lock;
	pthread_t		*thread;
}				t_simulation;

typedef struct s_philosopher
{
	int				id;
	int				times_eaten;
	long			initiation_time;
	long			last_meal_time;
	long			time_passed;
	t_simulation	*sim;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}				t_philosopher;

//routine
void	*routine(void *arg);
void	eat(t_philosopher *philo);
int		occupied_with(t_philosopher *philo, int duration, char *msg);
int		get_forks(t_philosopher *philo);
int		right_fork(t_philosopher *philo);
int		left_fork(t_philosopher *philo);
int		eaten_enough(t_philosopher *philo);
void	think(t_philosopher *philo);
int		dead_check(t_philosopher *philo);

//arg_check
int		arg_check(int argc, char **argv);

//utils
void	wait_threads(t_simulation *sim);
void	write_status(char *str, t_philosopher *philo);
int		ft_atoi(const char *str);
long	get_time(void);

//free
void	free_all(t_simulation *sim);

#endif