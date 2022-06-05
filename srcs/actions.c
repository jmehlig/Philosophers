/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:32:19 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/05 16:55:14 by jmehlig          ###   ########.fr       */
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
	// while (j < times.num_philos)
	// {
	// 	pthread_mutex_destroy(&(times.forks[j]));
	// 	j++;
	// }
	pthread_mutex_destroy(&(times.mutex));
	pthread_mutex_destroy(&(times.print));
}

bool	try_eating(t_philo philo, t_times *times)
{
	if (times->fork_states[philo.left_fork] == false && times->fork_states[philo.right_fork] == false)
	{
		//printf("test");
		pthread_mutex_lock(&(times->mutex));
		times->fork_states[philo.left_fork] = true;
		ft_print(*times, philo.number, TAKE_FORK);
		times->fork_states[philo.right_fork] = true;
		ft_print(*times, philo.number, TAKE_FORK);
		philo.state = EATING;
		ft_print(*times, philo.number, EATING);
		times->fork_states[philo.left_fork] = false;
		times->fork_states[philo.right_fork] = false;
		pthread_mutex_unlock(&(times->mutex));
		philo.time_since_meal = ft_time();
	//	go_sleeping(times->t_sleep);
		(philo.meals)++;
		return (true);
	}
	return (false);
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

	while (times->meal_counter < times->meals_to_eat && times->death == false)
	{
		i = 0;
		while (i < times->num_philos)
		{
			if (ft_time() - philo[i].time_since_meal > times->t_die)
			{
				ft_print(*times, i, DYING);
				times->death = true;
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
	philo = (t_philo *)philo_in;
	times = philo->times;
	i = 0;
	if (philo[i].number % 2 == 0)
		usleep((times.t_eat - 10) * 1000); // Abhängig von den gegebenen Zeiten?
	while (times.death == false)
	{
		if (try_eating(philo[i], &times) == true)
		{
			printf("haaaaaalllo");
			if (times.meal_counter == times.meals_to_eat)
				break ;
			philo->state = SLEEPING;
			ft_print(times, philo[i].number, SLEEPING);
			go_sleeping(times.t_sleep);
			philo->state = THINKING;
			ft_print(times, philo[i].number, THINKING);
			i++;
		}
	}
	check_if_died(&times, philo);
	ft_stop(times, philo);
	return (NULL);
}

int	ft_lonely(t_times times)
{
	ft_print(times, 1, DYING);
	return (0);
}
