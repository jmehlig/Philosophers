/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 11:16:56 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 15:06:26 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_time(t_data *data)
{
	long long		res;
	struct timeval	current;

	gettimeofday(&current, NULL);
	res = (current.tv_sec - data->time_begin.tv_sec) * 1000;
	res += (current.tv_usec - data->time_begin.tv_usec) / 1000;
	return (res);
}

void	ft_sleep(long long time, t_data *data)
{
	long long	current;

	current = ft_time(data);
	while (current < time)
	{
		usleep(200);
		current = ft_time(data);
	}
}

void	end(t_data *data)
{
	pthread_mutex_lock(&data->mutex);
	data->stopped_eating = true;
	pthread_mutex_unlock(&data->mutex);
}

int	not_all_ate(t_data *data)
{
	pthread_mutex_lock(&data->mutex);
	if (data->stopped_eating)
	{
		pthread_mutex_unlock(&data->mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->mutex);
	return (1);
}

bool	check_if_died(t_philo *philo)
{
	if (ft_time(philo->data) < philo->t_will_die)
		return (false);
	else
		return (true);
}
