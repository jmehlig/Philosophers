/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:32:19 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/16 17:21:18 by jmehlig          ###   ########.fr       */
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
			print(times, philo->number, SLEEPING);
			go_sleeping(times->t_sleep, times);
			print(times, philo->number, THINKING);
			i++;
		}
	}
	else
		usleep(30);
	return (NULL);
}

