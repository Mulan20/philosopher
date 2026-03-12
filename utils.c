#include "philo.h"

long long get_time(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void ft_usleep(int ms)
{
	long long start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(100);
}

void print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->printMutex);
	// pthread_mutex_lock(&philo->data->dieMutex);

	if (!philo->data->stop_simulation)
	{
		printf("%lld %d %s\n",
			get_time() - philo->data->start_time,
			philo->philo_id,
			msg);
	}

	// pthread_mutex_unlock(&philo->data->dieMutex);
	pthread_mutex_unlock(&philo->data->printMutex);
}