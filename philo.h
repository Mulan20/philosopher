#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_data t_data;

typedef struct s_philo
{
	int philo_id;
	int eaten_num_meals;
	long long last_meal;

	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;

	pthread_t thread;
	t_data *data;

}	t_philo;

typedef struct s_data
{
	int num_philo;
	int time_to_die;
	int time_to_sleep;
	int time_to_eat;
	int min_must_eat;

	int stop_simulation;

	long long start_time;

	pthread_mutex_t *forks;
	pthread_mutex_t printMutex;
	pthread_mutex_t dieMutex;

	t_philo *philos;
	pthread_t monitor;

}	t_data;

/* utils */
long long	get_time(void);
void		ft_usleep(int ms);
void		print_status(t_philo *philo, char *msg);

/* libft */
int			ft_atoi(const char *str);

/* init */
int			init_data(t_data *data, int ac, char **av);
void		init_philo(t_data *data);
int			init_all(t_data *data, int ac, char **av);

/* routine */
int			stop_simu(t_philo *philo);
void		philo_eat(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		philo_think(t_philo *philo);
void		*philo_routine(void *arg);

/* monitor */
void		*monitor_routine(void *arg);

/* main helpers */
int			validate_arguments(int ac, char **av);
int			create_all_thread(t_data *data);
void		join_all_threads(t_data *data);
void		cleanup(t_data *data);

#endif