/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:32:19 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/05 13:43:26 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	go_sleeping(int t_sleep)
{
	long long i;

	i = ft_time();
	while (t_sleep < ft_time() - i)
		usleep(5);
}

void	ft_stop(t_times times, t_philo *philo)
{
	int	i;
	int	j;

	i = 0;
	j = 0;

	while (i < times.num_philos)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	while (j < times.num_philos)
	{
		pthread_mutex_destroy(&(times.forks[j]));
		j++;
	}
	pthread_mutex_destroy(&(times.print));
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
	go_sleeping(times->t_sleep);
	pthread_mutex_unlock(&(times->forks[philo.left_fork]));
	pthread_mutex_unlock(&(times->forks[philo.right_fork]));
	(philo.meals)++;
}

int	ft_start(t_philo *philos, t_times times)
{
	t_thread	*threads;
	int			i;
	int			res;

	//times.philos = philos;
	threads = malloc (sizeof(t_thread) * times.num_philos);
	if (!threads)
		return (1);
	times.time_begin = ft_time();
	i = 0;
	while (i < times.num_philos)
	{
		threads[i].num = i;
		philos[i].times = times;
		res = pthread_create(&threads[i].thread, NULL, p_routine, &philos[i]);
		if (res != 0)
			return (exit_error("Thread create problem"));
		philos[i].thread = threads[i].thread;
		i++;
	}
	check_if_died(&times, philos);
	return (0);
}

void	ft_print(t_times times, int num, t_state state)
{
	long long	time;

	time = ft_time() - times.time_begin;
	pthread_mutex_lock(&(times.print));
	if (state == DYING)
		printf("%llims  %i   has    died\n", time, num);
	else if (state == TAKE_FORK)
		printf("%llims  %i   has   taken a fork\n", time, num);
	else if (state == EATING)
		printf("%llims  %i   is    eating\n", time, num);
	else if (state == SLEEPING)
		printf("%llims  %i   is    sleeping\n", time, num);
	else if (state == THINKING)
		printf("%llims  %i   is    thinking\n", time, num);
	pthread_mutex_unlock(&(times.print));
}

void	check_if_died(t_times *times, t_philo *philo)
{
	int	i;

	while (times->meal_counter < times->meals_to_eat && times->death == 0)
	{
		i = 0;
		while (i < times->num_philos)
		{
			if (ft_time() - philo[i].time_since_meal > times->t_die)
			{
				ft_print(*times, i, DYING);
				times->death = 1;
			}
			i++;
			//usleep(50);
		}
		i = 0;
		while (times->meals_to_eat != -1 && i < times->num_philos && philo[i].meals >= times->meal_counter)
			i++;
		if (i == times->num_philos)
			times->meal_counter++;
	}
}

void	*p_routine(void *philo_in)
{
	t_philo	*philo;
	t_times	times;
	int		i;

	//write(1, "here\n", 5);
	philo = (t_philo *)philo_in;
	times = philo->times;
	i = 0;
	if (philo[i].number % 2 != 0) //Wenn ich von 0 anfange, können ungerade nicht nebeneinander sitzen
	{
		while (times.death == 0)
		{
			try_eating(philo[i], &times);
			if (times.meal_counter == times.meals_to_eat)
				break ;
			ft_print(times, philo[i].number, SLEEPING);
			go_sleeping(times.t_sleep);
			ft_print(times, philo[i].number, THINKING);
			i++;
		}
	}
	else
		usleep(times.t_sleep * 1000); // Abhängig von den gegebenen Zeiten?
	check_if_died(&times, philo);
	ft_stop(times, philo);
	return (NULL);
}

int	ft_lonely(t_times times)
{
	ft_print(times, 1, DYING);
	return (0);
}
