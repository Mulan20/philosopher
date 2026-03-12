#include "philo.h"

//we will need to initialize all the variables in data struct
//*data so that the original data in main.c would be manipulated directly
int init_data(t_data *data, int ac, char **av)
{
	//order of the input 
	//->./a.out number_of_philosophers time_to_die time_to_eat time_to_sleep
	//[number_of_times_each_philosopher_must_eat]
	//
	data->num_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->time_to_eat = ft_atoi(av[3]);
	data->min_must_eat = (ac == 6) ? ft_atoi(av[5]) : -1; //special if condition
	data->stop_simulation = 0;
	data->start_time = get_time(); //get that time which was current

	/*
	 * now, time to initialize the mutexes 
	 * pthread_mutex_init(mutex, NULL) is used to initialize the mutex*/

	pthread_mutex_init(&data->printMutex, NULL);
	pthread_mutex_init(&data->dieMutex, NULL);

	//for mutex array we will need to use loop 
	//first get the number of forks form the number of philosophers
	//then malloc the space to store all the fork and initialize these in a loop 
	
	//num of philo * sizeof mutex 
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philo);
	if (!data->forks)
		return 0; //if memory allocation is not successful
	
	//now, initialize
	for (int i = 0; i < data->num_philo; i++)
	{
		pthread_mutex_init(&data->forks[i], NULL);
	}

	//now, create the space for the philosophers
	data->philos = malloc(sizeof(t_philo) * data->num_philo);
	if (!data->philos)
		return 0;
	return 1;
}

//to initialize the philosophers, we need data as we need to know the number of philosophers and the malloc is in there as well 
void init_philo(t_data *data)
{
	for (int i = 0; i < data->num_philo; i++)
	{
		data->philos[i].philo_id = i + 1;
		data->philos[i].eaten_num_meals = 0;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philo];
	}
}

//one function to initiate both at once
int init_all(t_data *data, int ac, char **av)
{
	if (!init_data(data, ac, av))
		return 0;
	init_philo(data);
	return 1;
}