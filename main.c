#include "philo.h"

int validate_arguments(int ac, char **av)
{
	int i;
	int j;
	if (ac < 5 || ac > 6)
	{
		printf("Error input\n");
		return 1;
	}
	i = 1; //we will start after a.out
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] <'0' || av[i][j] > '9')
			{
				printf("wrong input type\n");
				return 0;
			}
			j++;
		}
		if (ft_atoi(av[i]) <= 0)
		{
			printf("Positive number only \n");
			return 0;
		}
		i++;
	}
	return 1;
}

// int create_all_thread(t_data *data)
// {
// 	int i;
// 	i = 0;
// 	while (i < data->num_philo)
// 	{
// 		if (pthread_create(&data->philos[i].thread, NULL, philo_routine, &data->philos[i]) != 0)
// 		{
// 			printf("failed to create philo thread\n");
// 			return 0;
// 		}
// 		i++;
// 	}
// 	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
// 	{
// 		printf("Error : failed to create monitor threads!\n");
// 		return 0;
// 	}
// 	return 1;
// }

int create_all_thread(t_data *data)
{
	int i;

	i = 0;
	while (i < data->num_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
			philo_routine, &data->philos[i]) != 0)
			return (0);
		i++;
	}

	if (pthread_create(&data->monitor, NULL,
		monitor_routine, data) != 0)
		return (0);

	return (1);
}

void join_all_threads(t_data *data)
{
	int i = 0; // to go through all the philosophers
	while (i < data->num_philo)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
}

void cleanup (t_data *data)
{
	int i; //counter to delete all the forks in array

	if (data->forks)
	{
		i = 0;
		while (i < data->num_philo) //as the num of fork and philosopher is the same 
		{
			pthread_mutex_destroy(&data->forks[i]); //now, we delete that particular mutex fork
			i++;//don't forget incrementation

		}
		free(data->forks); //first, delete all the mutex first, then, free the mutex array slot 
	}

	//two more mutex to clear -> for print and death
	pthread_mutex_destroy(&data->printMutex);
	pthread_mutex_destroy(&data->dieMutex);
	//if there are philosophers stored, we will free that 
	if (data->philos)
	{
		free(data->philos);
	}
}
int main(int ac, char **av)
{
	t_data data;

	if (!validate_arguments(ac, av))
		return 1;
	if (!init_all(&data, ac, av))
		return 1;

	//now, let's create the philosopher threads here
	if (!create_all_thread(&data))
	{
		cleanup(&data);
		return 1;
	}

	join_all_threads(&data);
	cleanup(&data);
	return 0;
}