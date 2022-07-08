/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 11:16:41 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 17:39:00 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_print_died(t_philo *philo, long long time, t_state state)
{
	bool	temp;

	pthread_mutex_lock(&philo->data->print);
	temp = philo->is_dead;
	pthread_mutex_unlock(&philo->data->print);
	pthread_mutex_lock(&philo->data->print);
	if (state == DYING && temp == false)
		printf("\e[;31m%6llims  %i   was taken by the reaper\n",
			time, philo->number);
	pthread_mutex_unlock(&philo->data->print);
	return (1);
}

int	ft_print(t_philo *philo, long long time, t_state state)
{
	pthread_mutex_lock(&philo->data->mutex);
	if (philo->data->stopped_eating)
	{
		pthread_mutex_unlock(&philo->data->mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->mutex);
	pthread_mutex_lock(&philo->data->print);
	if (state == TAKE_FORK)
		printf("%6llims  %3i   has   taken a fork\n", time, philo->number);
	else if (state == EATING)
		printf("%6llims  %3i   is    eating\n", time, philo->number);
	else if (state == SLEEPING)
		printf("%6llims  %3i   is    sleeping\n", time, philo->number);
	else if (state == THINKING)
		printf("%6llims  %3i   is    thinking\n", time, philo->number);
	pthread_mutex_unlock(&philo->data->print);
	return (1);
}

int	check_all_ate(t_data *data)
{
	while (not_all_ate(data))
	{
		pthread_mutex_lock(&data->mutex);
		if (data->meals_to_eat && data->num_philos == data->philos_ate_enough)
		{
			printf("\e[;32m%6llums Everyone has finished eating\n"
				"Just this once - Everybody lived!\n", ft_time(data));
			pthread_mutex_unlock(&data->mutex);
			return (0);
		}
		pthread_mutex_unlock(&data->mutex);
		usleep(50);
	}
	return (1);
}
