/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nandreev <nandreev@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:04:13 by nandreev          #+#    #+#             */
/*   Updated: 2024/10/23 16:09:05 by nandreev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_msg(char *msg)
{
	printf("%s\n", msg);
	return (0);
}

int	arg_check(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	if (argc != 5 && argc != 6)
		return (error_msg("Error: wrong number of arguments"));
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (error_msg("Error: invalid argument"));
			j++;
		}
		if (ft_atoi(argv[i]) < 1)
			return (error_msg("Error: invalid argument"));
		i++;
	}
	return (1);
}
