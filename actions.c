/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:32:19 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/04 12:27:22 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	go_sleeping(int t_sleep, t_times *times)
{
	long long i;

	i = ft_time();
	usleep(50);
}

void	ft_stop(t_times *times, t_philo *philo)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < times->num_philos)
	{
		pthread_join(philo[i].thread.thread, NULL);
		i++;
	}
	while (i < times->num_philos)
		pthread_mutex_destroy(&(times->forks[i]));
	pthread_mutex_destroy(&(times->print));
}

void	try_eating(t_philo philo, t_times *times)
{
	pthread_mutex_lock(&(times->forks[philo.left_fork]));
	ft_print(*times, philo.number, TAKE_FORK);
	pthread_mutex_lock(&(times->forks[philo.right_fork]));
	ft_print(*times, philo.number, TAKE_FORK);
	pthread_mutex_lock(&(times->is_eating));
	ft_print(*times, philo.number, EATING);
	philo.time_since_meal = ft_time();
	pthread_mutex_unlock(&(times->is_eating));
	go_sleeping(times->t_sleep, times);
	pthread_mutex_unlock(&(times->forks[philo.left_fork]));
	pthread_mutex_unlock(&(times->forks[philo.right_fork]));
	(philo.meals)++;
}

int	ft_start(t_philo *philos, t_times times)
{
	t_thread	threads[times.num_philos];
	int			i;
	int			res;

	times.philos = philos;
	times.time_begin = ft_time();
	i = 0;
	while (i < times.num_philos)
	{
		threads[i].num = i + 1;
		res = pthread_create(&threads[i].thread, NULL, p_routine, &threads[i]);
		if (!res)
			return (exit_error("Thread create problem"));
		philos[i].thread = threads[i];
		i++;
	}
	check_death(threads, times, philos);
	retuen (0);
}

void	ft_print(t_times times, int num, t_state state)
{
	int	time;

	time = times.time_begin;
	pthread_mutex_lock(&(times.print));
	if (state == DYING)
		printf("%ims  %i   has    died\n", time, num);
	else if (state == TAKE_FORK)
		printf("%ims  %i   has   taken a fork\n", time, num);
	else if (state == EATING)
		printf("%ims  %i   is    eating\n", time, num);
	else if (state == SLEEPING)
		printf("%ims  %i   is    sleeping\n", time, num);
	else if (state == THINKING)
		printf("%ims  %i   is    thinking\n", time, num);
	pthread_mutex_unlock(&(times.print));
}

void	check_if_died(t_times *times, t_philo *philo)
{
	int	i;

	while (!(times->meal_counter == times->meals_to_eat || times->death))
	{
		i = 0;
		while (i <times->num_philos)
		{
			if (philo[i].time_since_meal - ft_time() > times->t_die)
			{
				ft_print(*times, i, DYING);
				times->death = 1;
			}
			usleep(50);
		}
		i = 0;
		while (times->meals_to_eat != INT_MAX && i < times->num_philos && philo[i].meals >= times->meal_counter)
			i++;
		if (i == times->num_philos)
			times->meal_counter++;
	}
	ft_stop(times, philo);
}

void	*p_routine(void *philo_in)
{
	t_philo	*philo;
	t_times	*times;
	int		i;

	philo = (t_philo *)philo_in;
	*times = philo->times;
	i = 0;
	if (philo[i].number % 2 != 0) //Wenn ich von 0 anfange, können ungerade nicht nebeneinander sitzen
	{
		while (!times->death)
		{
			try_eating(philo[i], times);
			if (times->meal_counter == times->meals_to_eat)
				break ;
			ft_print(*times, philo[i].number, SLEEPING);
			go_sleeping(times->t_sleep, times);
			ft_print(*times, philo[i].number, THINKING);
			i++;
		}
	}
	else
		usleep(22222); // Abhängig von den gegebenen Zeiten?
	check_if_died(times, philo);
	return (NULL);
}

