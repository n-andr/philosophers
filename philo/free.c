#include "philo.h"

void	free_struct(t_simulation *data)
{
	
	free(data->philosopher);
	free(data->forks);
	free(data);
}