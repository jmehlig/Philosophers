/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 11:16:03 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 15:05:06 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eats(t_philo *philo)
{
	if (!pick_up_left_fork(philo) || !pick_up_right_fork(philo))
		return (philo_dies(philo));
	ft_sleep(philo->time_since_meal + philo->data->t_eat, philo->data);
	pthread_mutex_lock(&philo->right_fork->mutex);
	philo->right_fork->in_use = false;
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_lock(&philo->left_fork->mutex);
	philo->left_fork->in_use = false;
	pthread_mutex_unlock(&philo->left_fork->mutex);
	return (1);
}

int	philo_thinks(t_philo *philo)
{
	long long	t_since_meal;

	if (!not_all_ate(philo->data))
		return (0);
	ft_print(philo, ft_time(philo->data), THINKING);
	t_since_meal = (ft_time(philo->data) - philo->time_since_meal);
	return (1);
}

int	philo_sleeps(t_philo *philo)
{
	long long	t_begin;

	t_begin = ft_time(philo->data);
	if (!not_all_ate(philo->data))
		return (0);
	ft_print(philo, t_begin, SLEEPING);
	if (t_begin + philo->data->t_sleep >= philo->t_will_die)
	{
		ft_sleep(philo->t_will_die, philo->data);
		return (philo_dies(philo));
	}
	ft_sleep(t_begin + philo->data->t_sleep, philo->data);
	return (1);
}

int	philo_dies(t_philo *philo)
{
	if (check_if_died(philo) == false || !not_all_ate(philo->data))
		return (0);
	pthread_mutex_lock(&philo->die_not_eat);
	end(philo->data);
	pthread_mutex_unlock(&philo->die_not_eat);
	ft_print_died(philo, philo->t_will_die, DYING);
	philo->is_dead = 1;
	return (0);
}
