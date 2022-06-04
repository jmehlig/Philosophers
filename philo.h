/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 13:09:20 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/04 12:22:52 by jmehlig          ###   ########.fr       */
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

typedef enum e_state
{
	THINKING,
	TAKE_FORK,
	EATING,
	SLEEPING,
	DYING
}	t_state;

typedef struct s_philo
{
	t_state		state;
	int			number;
	int			left_fork;
	int			right_fork;
	long long	time_since_meal;
	int			*fork_used;
	int			meals;
	t_times		times;
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
	int			death;
	pthread_mutex_t	forks[250];
	pthread_mutex_t	print;
	pthread_mutex_t	meal_check; //NAME ÄNDERN!!!
	t_philo		*philos;
}				t_times;

typedef struct s_thread
{
	pthread_t	thread;
	int			num;
}				t_thread;

//philo.c
void		start_mutex(t_times *times);
int			ft_init(int argc, char *argv[], t_times times);
t_philo		init_philo(int num, t_philo *philos, t_times times);
long long	ft_time(void);

//utils.c
int			exit_error(char *mes);
int			ft_atoi(const char *str);

//actions.c
int			ft_start(t_philo *philos, t_times times);
void		ft_print(t_times times;, int num, t_state state);
void		*p_routine(void *philo_in);

#endif
