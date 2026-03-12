#include "philo.h"

int stop_simu(t_philo *philo)
{
	int stop;

	pthread_mutex_lock(&philo->data->dieMutex);
	stop = philo->data->stop_simulation;
	pthread_mutex_unlock(&philo->data->dieMutex);

	return stop;
}

static void take_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

static void release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void philo_eat(t_philo *philo)
{
	take_forks(philo);

	pthread_mutex_lock(&philo->data->dieMutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->dieMutex);

	print_status(philo, "is eating");

	ft_usleep(philo->data->time_to_eat);

	philo->eaten_num_meals++;

	release_forks(philo);
}

void philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void philo_think(t_philo *philo)
{
	print_status(philo, "is thinking");
}

void *philo_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;


    // special case: only one philosopher
	if (philo->data->num_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");

		ft_usleep(philo->data->time_to_die);

		pthread_mutex_unlock(philo->left_fork);
		print_status(philo, "died");

		return NULL;
	}

	if (philo->philo_id % 2 == 0)
		ft_usleep(100);

	while (!stop_simu(philo))
	{
		philo_eat(philo);

		if (philo->data->min_must_eat != -1 &&
			philo->eaten_num_meals >= philo->data->min_must_eat)
			break;

		philo_sleep(philo);
		philo_think(philo);
	}
	return NULL;
}