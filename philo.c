/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 13:08:51 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/16 17:29:34 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Join threads???
// Destroy threads?

int	main(int argc, char *argv[])
{
	t_times	times;
	int		i;
	int		j;

	if (!(argc == 5 || argc == 6))
		return (exit_error("Not the right input"));
	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] >= '0' && argv[i][j] <= '9')
				j++;
			else
				return (exit_error("Should be numeric arguments."));
		}
		i++;
	}
	times.num_philos = ft_atoi(argv[1]);
	if(ft_init(argc, argv, times) == 1)
		return (exit_error("Argument out of range"));
}

void	start_mutex(t_times *times)
{
	int	i;

	i = times->num_philos - 1;
	while (i >= 0)
	{
		if (pthread_mutex_init(&(times->forks), NULL))
			exit_error("Creating mutex went wrong");
		i--;
	}
	if (pthread_mutex_init(&(times->writing), NULL))
		exit_error("Creating mutex went wrong");
	if (pthread_mutex_init(&(times->meal_check), NULL))
		exit_error("Creating mutex went wrong");
}

int	ft_init(int argc, char *argv[], t_times times)
{
	t_philo	philos[times.num_philos];
	int		forks[times.num_philos];
	int		i;

	times.forks = forks;
	times.time_begin = ft_time();
	times.t_die = ft_atoi(argv[2]);
	times.t_eat = ft_atoi(argv[3]);
	times.t_sleep = ft_atoi(argv[4]);
	times.meal_counter = 0;
	if (times.t_eat > times.t_die)
		times.t_eat = times.t_die;
	if (times.t_sleep > times.t_die)
		times.t_sleep = times.t_die;
	if (argv[5])
	{
		times.meals_to_eat = ft_atoi(argv[5]);
		if (times.meals_to_eat <= 0)
			return (1);
	}
	else
		times.meals_to_eat = -1;
	if (times.num_philos < 2 || times.t_die < 0 || times.t_eat < 0 || times.t_sleep < 0)
		return (1);
	start_mutex(&times);
	i = 0;
	while (i < times.num_philos)
	{
		philos[i] = init_philo(i, philos, times);
		if (i == times.num_philos)
			philos[i].left_fork = 0;
		i++;
	}
	ft_start(philos, times);
	return (0);
}

// forks?
t_philo	init_philo(int num, t_philo *philos, t_times times)
{
	t_philo	phil;

	phil.time_since_meal = ft_time();
	phil.left_fork = num;
	phil.right_fork = num + 1;
	phil.number = num;
	phil.state = THINKING;
	phil.meals = 0;
	phil.times = times;
	return (phil);
}

long long	ft_time(void)
{
	struct timeval	t;

	if(!gettimeofday(&t, NULL));
		return (exit_error("timestamp went wrong"));
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}
