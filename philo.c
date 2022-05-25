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

	times.num_philos = ft_atoi(argv[1]);
	if (!(argc == 5 || argc == 6))
		return (exit_error("Not the right input"));
	i = 0;
	while (i < argv)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] >= '0' && argv[i][j] <= '9')
				j++;
			else
				exit_error("Should be numeric arguments.");
		}
		i++;
	}
	ft_init(argc, argv, times);
}

void	ft_init(int argc, char *argv[], t_times times)
{
	t_philo	philos[times.num_philos];
	int		forks[times.num_philos];
	int		i;

	i = 0;
	while (i < times.num_philos)
	{
		philos[i] = init_philo(i + 1, philos);
		i++;
	}
	times.forks = forks;
	times.time_begin = ft_time();
	times.t_die = ft_atoi(argv[2]);
	times.t_eat = ft_atoi(argv[3]);
	times.t_sleep = ft_atoi(argv[4]);
	if (times.t_eat > times.t_die)
		times.t_eat = times.t_die;
	if (times.t_sleep > times.t_die)
		times.t_sleep = times.t_die;
	if (argv[5])
	{
		times.meals_to_eat = ft_atoi(argv[5]);
		times.meal_counter = 0;
	else
		times.meals_to_eat = -1;
	ft_start(philos, times);
}

// forks?
init_philo(int num, t_philo *philos)
{
	t_philo	phil;

	phil.time_since_meal = ft_time();
	phil.left_fork = 0;
	phil.right_fork = 0;
	phil.number = num;
	phil.state = 0;
	phil.meals = 0;
	return (phil);
}

long long	ft_time(void)
{
	struct timeval	t;

	if(!gettimeofday(&t, NULL));
		return (exit_error("timestamp went wrong"));
	return ((t->tv_sec * 1000) + (t->tv_usec / 1000));
}
