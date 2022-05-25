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

void	ft_start(t_philo *philos, t_times times)
{
	t_thread	threads[times.num_philos];
	int			i;
	int			res;

	times.philos = philos;
	i = 0;
	while (i < times.num_philos)
	{
		threads[i] = i + 1;
		res = pthread_create(&threads[i].thread, NULL, p_routine, &threads[i]);
		if (!res)
			return (exit_error("Thread create problem"));
	}
	check_death(threads, times, philos);
}

void	*p_routine(void *philo_in)
{
	t_philo	*philo;

	philo = (t_philo *)philo_in;
	if (philo.number % 2 == 0)
	{
		if ()
		philo.right_fork = 1;
	}

}

