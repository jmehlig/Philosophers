/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 11:16:17 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 17:56:37 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	reset_time(t_philo *philo)
{
	philo->time_since_meal = ft_time(philo->data);
	philo->t_will_die = philo->time_since_meal + philo->data->t_die;
	philo->times_ate++;
	if (!not_all_ate(philo->data))
		return (0);
	ft_print(philo, ft_time(philo->data), TAKE_FORK);
	ft_print(philo, ft_time(philo->data), EATING);
	return (1);
}

int	pick_up_left_fork(t_philo *philo)
{
	while (true)
	{
		pthread_mutex_lock(&philo->die_not_eat);
		if (check_if_died(philo) == true)
		{
			pthread_mutex_unlock(&philo->die_not_eat);
			return (philo_dies(philo));
		}
		pthread_mutex_unlock(&philo->die_not_eat);
		pthread_mutex_lock(&philo->left_fork->mutex);
		if (philo->left_fork->in_use)
		{
			pthread_mutex_unlock(&philo->left_fork->mutex);
			usleep(200);
			continue ;
		}
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_lock(&philo->left_fork->mutex);
		philo->left_fork->in_use = true;
		pthread_mutex_unlock(&philo->left_fork->mutex);
		ft_print(philo, ft_time(philo->data), TAKE_FORK);
		return (not_all_ate(philo->data));
	}
}

int	pick_up_right_fork(t_philo *philo)
{
	while (true)
	{
		pthread_mutex_lock(&philo->die_not_eat);
		if (check_if_died(philo) == true)
		{
			pthread_mutex_unlock(&philo->die_not_eat);
			return (philo_dies(philo));
		}
		pthread_mutex_unlock(&philo->die_not_eat);
		pthread_mutex_lock(&philo->right_fork->mutex);
		if (philo->right_fork->in_use)
		{
			pthread_mutex_unlock(&philo->right_fork->mutex);
			usleep(50);
			continue ;
		}
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_lock(&philo->right_fork->mutex);
		philo->right_fork->in_use = true;
		pthread_mutex_unlock(&philo->right_fork->mutex);
		return (reset_time(philo));
	}
}
