#include "philo.h"

void *monitor_routine(void *arg)
{
	t_data *data;
	int i;
	long long time_since;

	data = (t_data *)arg;

	while (1)
	{
		i = 0;
		while (i < data->num_philo)
		{
			pthread_mutex_lock(&data->dieMutex);
			time_since = get_time() - data->philos[i].last_meal;
			pthread_mutex_unlock(&data->dieMutex);

			if (time_since > data->time_to_die)
			{
				pthread_mutex_lock(&data->dieMutex);
				data->stop_simulation = 1;
				pthread_mutex_unlock(&data->dieMutex);

				print_status(&data->philos[i], "died");
				return NULL;
			}
			i++;
		}
		usleep(200);
	}
	return NULL;
}