/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:40:58 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/22 23:20:35 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status(char *str, t_philosopher *philo)
{
	pthread_mutex_lock(&philo->sim->print_lock); // maybe move to routine
	if (philo->sim->is_dead != 1)
		printf("%ld %d %s\n", get_time() - philo->initiation_time, philo->id + 1, str);
	pthread_mutex_unlock(&philo->sim->print_lock); // maybe move to routine
}

void	wait_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philo)
	{
		pthread_join(sim->thread[i], NULL);
		i++;
	}
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

long	get_time(void)
{
	struct timeval	time;
	long			ms;

	gettimeofday(&time, NULL);
	ms = time.tv_sec * 1000;
	ms += time.tv_usec / 1000;
	return (ms);
}