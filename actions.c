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
		i++;
	}
	check_death(threads, times, philos);
	retuen (0);
}

void	ft_print(t_times times, int num, t_state state)
{
	int	time;

	time = times.time_begin;
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
}

void	*p_routine(void *philo_in)
{
	t_philo	*philo;
	t_times	*times;
	int		i;

	philo = (t_philo *)philo_in;
	*times = philo->times;
	i = 0;
	if (philo->number % 2 != 0) //Wenn ich von 0 anfange, können ungerade nicht nebeneinander sitzen
	{
		while (!times->death)
		{
			try_eating(philo, times);
			if (times->meal_counter == times->meals_to_eat)
				break ;
			ft_print(times, philo->number, SLEEPING);
			go_sleeping(times->t_sleep, times);
			ft_print(times, philo->number, THINKING);
			i++;
		}
	}
	else
		usleep(33333);
	return (NULL);
}

