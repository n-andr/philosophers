/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:49 by nandreev          #+#    #+#             */
/*   Updated: 2024/09/23 00:23:36 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_simulation t_simulation;

typedef struct	s_simulation
{
	int		num_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		ate_enouth; //	int	philo_finish;
	int		is_dead;
	//int		num_eat;
	int		must_eat;
	int 	*fork_status;
	pthread_mutex_t *forks;
	pthread_mutex_t print_lock; //for writing in stdout (pthread_mutex_t write;)
	pthread_t 		*thread;

	//t_philosopher	*philosopher;
}				t_simulation;

typedef struct	s_philosopher
{
	int		id;
	int		times_eaten;
	long	initiation_time;
    long 	last_meal_time;
	long	time_passed;
	t_simulation 	*sim;
	pthread_mutex_t left_fork;
	pthread_mutex_t right_fork;
}				t_philosopher;


//routine
void	*routine(void *arg);
//utils
void	wait_threads(t_simulation *sim);
void	write_status(char *str, t_philosopher *philo);
int		ft_atoi(const char *str);
long	get_time(void);

//free
void	free_all(t_simulation *sim);

#endif