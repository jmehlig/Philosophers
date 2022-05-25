/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 13:09:20 by jmehlig           #+#    #+#             */
/*   Updated: 2022/05/16 17:25:35 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

typedef struct s_philo
{
	int			state;
	int			number;
	int			left_fork;
	int			right_fork;
	long long	time_since_meal;
	int			*fork_used;
	int			meals;
	pthread_mutex_t	mutex;
}				t_philo;

typedef struct s_times
{
	int			t_die;
	int			t_eat;
	int			t_sleep;
	long long	time_begin;
	int			meal_counter;
	int			meals_to_eat;
	int			num_philos;
	int			*forks;
	t_philo		*philos;
}				t_times;

typedef struct s_thread
{
	pthread_t	thread;
	int			num;
}				t_thread;

//philo.c
void		ft_init(int argc, char *argv[], t_times times);
long long	ft_time(void);
void		init_philo(int num, t_philo *philos);

//utils.c
int			exit_error(char *mes);
int			ft_atoi(const char *str);

//actions.c
void		ft_start(t_philo *philos, t_times times);


#endif
