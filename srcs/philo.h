/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 11:16:34 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/14 18:01:47 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>

typedef enum e_state
{
	THINKING,
	TAKE_FORK,
	EATING,
	SLEEPING,
	DYING
}	t_state;

typedef struct s_data	t_data;

typedef struct s_fork	t_fork;

typedef struct s_philo
{
	int				number;
	int				is_dead;
	int				times_ate;
	long long		time_since_meal;
	long long		t_will_die;
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_t		thread;
	pthread_mutex_t	die_not_eat;
	struct s_data	*data;
}					t_philo;

struct s_fork
{
	bool			in_use;
	pthread_mutex_t	mutex;
};

struct		s_data
{
	int				num_philos;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				meals_to_eat;
	int				philos_ate_enough;
	bool			stopped_eating;
	struct timeval	time_begin;
	pthread_mutex_t	mutex;
	pthread_mutex_t	print;
	t_fork			*forks;
	t_philo			*philos;
};

// actions.c
int			philo_thinks(t_philo *philo);
int			philo_sleeps(t_philo *philo);
int			philo_dies(t_philo *philo);
int			philo_eats(t_philo *philo);

// eat_utils.c
int			reset_time(t_philo *philo);
int			pick_up_right_fork(t_philo *philo);
int			pick_up_left_fork(t_philo *philo);

// init.c
int			create_forks(t_fork **forks, t_data *data);
int			init_philos(t_philo **philos, t_data *data);

// philo.c
int			create_philos(t_philo **philos, t_data *data);
void		get_arguments(t_data *data, char **argv);
int			ft_lonely(t_data data);
int			ft_init(char **argv);

// printing.c
int			ft_print_died(t_philo *philo, long long time,
				t_state state);
int			ft_print(t_philo *philo, long long time,
				t_state state);
int			check_all_ate(t_data *data);

// routine.c
int			finish_eating(t_philo *philo);
void		p_routine(t_philo *philo);

//stop.c
void		destroy_mutexs(int n, t_fork *obj);
void		ft_stop(t_data *data);
int			exit_error(char *mes);
void		print_error(char *mes);

//time.c
long long	ft_time(t_data *data);
void		ft_sleep(long long wake_time, t_data *data);
void		end(t_data *data);
int			not_all_ate(t_data *data);
bool		check_if_died(t_philo *philo);

//utils.c
size_t		ft_strlen(const char *str);
long		ft_atoi(const char *str);

#endif
