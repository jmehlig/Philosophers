/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:06:00 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 15:05:39 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	finish_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex);
	if (philo->times_ate == philo->data->meals_to_eat)
	{
		philo->data->philos_ate_enough++;
		pthread_mutex_unlock(&philo->data->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->mutex);
	return (0);
}

void	p_routine(t_philo *philo)
{
	while (true)
	{
		if (!philo_eats(philo))
			break ;
		if (check_if_died(philo) == true)
		{
			philo_dies(philo);
			break ;
		}
		if (philo->data->meals_to_eat && finish_eating(philo))
			break ;
		if (!philo_sleeps(philo))
			break ;
		if (check_if_died(philo) == true)
		{
			philo_dies(philo);
			break ;
		}
		if (!philo_thinks(philo))
			break ;
	}
}
