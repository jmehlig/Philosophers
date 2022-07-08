/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 11:15:55 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 18:05:03 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_forks(t_fork **forks, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		(*forks)[i].in_use = false;
		if (pthread_mutex_init(&(*forks)[i].mutex, NULL) != 0)
		{
			destroy_mutexs(i, *forks);
			return (0);
		}
		i++;
	}
	return (1);
}

int	allocate_philos(t_data *data)
{
	data->forks = (t_fork *)malloc(sizeof(t_fork) * (data->num_philos));
	if (data->forks == NULL)
	{
		write(2, "Memory allocation failed\n", 25);
		return (0);
	}
	data->philos = (t_philo *)malloc(sizeof(t_philo) * (data->num_philos));
	if (data->philos == NULL)
	{
		write(2, "Memory allocation failed\n", 25);
		return (0);
	}
	return (1);
}

int	start_mutex(t_data *data)
{
	if (!create_forks(&data->forks, data))
	{
		free(data->philos);
		print_error("Mutex creation failed\n");
		return (0);
	}
	if (pthread_mutex_init(&data->print, NULL) != 0)
	{
		free(data->philos);
		destroy_mutexs(data->num_philos, data->forks);
		print_error("Mutex creation failed\n");
		return (0);
	}
	if (pthread_mutex_init(&data->mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print);
		destroy_mutexs(data->num_philos, data->forks);
		free(data->philos);
		print_error("Mutex creation failed\n");
		return (0);
	}
	return (1);
}

int	ft_init(char **argv)
{
	t_data	data;

	get_arguments(&data, argv);
	if (data.num_philos == 1)
		return (ft_lonely(data));
	if (argv[5] && data.meals_to_eat == 0)
		return (1);
	if (data.num_philos < 1 || data.t_die < 60
		|| data.t_eat < 60 || data.t_sleep < 60)
		return (1);
	if (allocate_philos(&data) == 0)
		return (1);
	if (start_mutex(&data) == 0)
		return (1);
	if (!create_philos(&data.philos, &data))
		return (1);
	check_all_ate(&data);
	ft_stop(&data);
	return (0);
}
