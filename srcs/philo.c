/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 11:16:25 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 15:22:30 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	set_philo(int i, t_philo *philo, t_data *data)
{
	if (pthread_mutex_init(&philo->die_not_eat, NULL) != 0)
	{
		print_error("Mutex creation failed\n");
		return (0);
	}
	philo->data = data;
	philo->number = i + 1;
	if (i % 2)
		usleep(100);
	philo->t_will_die = data->t_die;
	philo->left_fork = &data->forks[i];
	if (i == data->num_philos - 1)
		philo->right_fork = &data->forks[0];
	else
		philo->right_fork = &data->forks[i + 1];
	if (pthread_create(&philo->thread, NULL, (void *)&p_routine,
			(void *)philo) != 0)
	{
		print_error("Thread creation failed\n");
		return (0);
	}
	return (1);
}

int	create_philos(t_philo **philos, t_data *data)
{
	int	i;

	i = 0;
	if (gettimeofday(&data->time_begin, NULL) == -1)
	{
		ft_stop(data);
		print_error("Timestamp went wrong\n");
		return (0);
	}
	while (i < data->num_philos)
	{
		if (!set_philo(i, &(*philos)[i], data))
		{
			pthread_mutex_lock(&data->print);
			end(data);
			pthread_mutex_unlock(&data->print);
			pthread_mutex_lock(&data->print);
			print_error("An error occured while creating a philo\n");
			pthread_mutex_unlock(&data->print);
			ft_stop(data);
			return (0);
		}
		i++;
	}
	return (1);
}

void	get_arguments(t_data *data, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->num_philos = (unsigned int)ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->meals_to_eat = ft_atoi(argv[5]);
	data->stopped_eating = false;
}

int	ft_lonely(t_data data)
{
	printf("%6ims  %i   has   taken a fork\n", 0, 1);
	printf("\e[;31m%6ims  %i   was taken by the reaper\n",
		data.t_die, 1);
	return (0);
}

int	main(int argc, char *argv[])
{
	int	i;
	int	j;

	if (!(argc == 5 || argc == 6))
		return (exit_error("Not the right input"));
	i = 1;
	while (argv[i])
	{
		j = 0;
		if (j == 0 && argv[i][j] == '+')
			j++;
		while (argv[i][j] != '\0')
		{
			if ((argv[i][j] >= '0' && argv[i][j] <= '9'))
				j++;
			else
				return (exit_error("Should be numeric arguments."));
		}
		if (ft_atoi(argv[i]) > 2147483647)
			return (exit_error("Argument too large"));
		i++;
	}
	if (ft_init(argv) == 1)
		exit_error("Not the right input");
	return (0);
}
