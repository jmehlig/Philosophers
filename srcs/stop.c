/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 11:16:50 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 17:54:19 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexs(int n, t_fork *fork)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&fork[i].mutex);
		i++;
	}
	free(fork);
}

void	ft_stop(t_data *data)
{
	int	i;

	i = 0;
	usleep(100);
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	destroy_mutexs(data->num_philos, data->forks);
	free(data->philos);
	pthread_mutex_destroy(&data->mutex);
	pthread_mutex_destroy(&data->print);
}

int	exit_error(char *mes)
{
	printf("\e[;31m%s\n", mes);
	exit(EXIT_FAILURE);
}

void	print_error(char *mes)
{
	printf("\e[;31m%s\n", mes);
}
