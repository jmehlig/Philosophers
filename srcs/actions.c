/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:32:19 by jmehlig           #+#    #+#             */
/*   Updated: 2022/06/10 15:11:58 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	go_sleeping(int time, t_philo *philo, t_times *times)
{
	long long	i;
	long long	j;

	pthread_mutex_lock(&(times->mutex));
	i = ft_time();
	j = i;
	while (time > j - i)
	{
		//check_if_died(times, philo);
		printf("%i sleeping\n", philo->number + 1);
		usleep(100);
		j = ft_time();
	}
	pthread_mutex_unlock(&(times->mutex));
}

void	ft_stop(t_times *times)
{
	int	i;

	i = 0;
	while (i < times->num_philos)
	{
		pthread_join(times->philos[i]->thread, NULL);
		i++;
	}
	// while (j < times.num_philos)
	// {
	// 	pthread_mutex_destroy(&(times.forks[j]));
	// 	j++;
	// }
	pthread_mutex_destroy(&(times->mutex));
	pthread_mutex_destroy(&(times->print));
	pthread_mutex_destroy(&(times->is_eating));
}

void	philo_eat(t_philo *philo, t_times *times)
{
	int	i;

	i = 0;
	if (times->fork_states[philo->left_fork] == false && times->fork_states[philo->right_fork] == false)
	{
		//printf("test");
		pthread_mutex_lock(&(times->is_eating));
		times->fork_states[philo->left_fork] = true;
		ft_print(*times, philo->number, TAKE_FORK);
		times->fork_states[philo->right_fork] = true;
		pthread_mutex_unlock(&(times->is_eating));
		ft_print(*times, philo->number, TAKE_FORK);
		ft_print(*times, philo->number, EATING);
		if  (check_if_died(times, philo) == true)
			return ;
		go_sleeping(times->t_eat, philo, times);
		pthread_mutex_lock(&(times->mutex));
		printf("%i finished eating\n", philo->number + 1);
		pthread_mutex_unlock(&(times->mutex));
		pthread_mutex_lock(&(times->is_eating));
		times->fork_states[philo->left_fork] = false;
		times->fork_states[philo->right_fork] = false;
		pthread_mutex_unlock(&(times->is_eating));
		philo->time_since_meal = ft_time();
		philo->state = SLEEPING;
		(philo->meals)++;
		// while (times->meals_to_eat != -1 && i < times->num_philos && times->philos[philo->number]->meals >= times->meal_counter)
		// 	i++;
		// pthread_mutex_lock(&(times->mutex));
		// if (i == times->num_philos)
		// 	times->meal_counter++;
		// pthread_mutex_unlock(&(times->mutex));
	}
	else
		usleep(50);
}

int	ft_start(t_philo **philos, t_times times)
{
	int			i;
	int			res;

	//times.philos = philos;
	times.time_begin = ft_time();
	i = 0;
	times.philos = philos;
	times.lock = true;
	while (i < times.num_philos)
	{
		philos[i]->times = times;
		//write(1, "here\n", 4);
		res = pthread_create(&philos[i]->thread, NULL, &p_routine, (void *)philos[i]);
		if (res != 0)
			return (exit_error("Thread create problem"));
		i++;
	}
	times.lock = false;
	check_if_died(&times, *philos);
	return (0);
}

void	ft_print(t_times times, int num, t_state state)
{
	long long	time;

	time = ft_time() - times.time_begin;
	pthread_mutex_lock(&(times.print));
	if (state == DYING)
		printf("%llims  %i   has    died\n", time, num + 1);
	else if (state == TAKE_FORK)
		printf("%llims  %i   has   taken a fork\n", time, num + 1);
	else if (state == EATING)
		printf("%llims  %i   is    eating\n", time, num + 1);
	else if (state == SLEEPING)
		printf("%llims  %i   is    sleeping\n", time, num + 1);
	else if (state == THINKING)
		printf("%llims  %i   is    thinking\n", time, num + 1);
	pthread_mutex_unlock(&(times.print));
}

bool	check_if_died(t_times *times, t_philo *philo)
{
	if (times->t_die < ft_time() - philo->time_since_meal)
	{
		philo->state = DYING;
		return (true);
	}
	return (false);
}

//erst wenn alle Threads created sind, soll es losgehen
bool	ft_lock_sleep(t_times *times)
{
	pthread_mutex_lock(&(times->mutex));
	if (times->lock == true)
		usleep(5);
	pthread_mutex_unlock(&(times->mutex));
	return (times->lock);
}

void	philo_sleep(t_philo *philo, t_times *times)
{
	ft_print(*times, philo->number, SLEEPING);
	philo->state = THINKING;
	go_sleeping(times->t_sleep, philo, times);
}

void	philo_think(t_philo *philo, t_times *times)
{
	ft_print(*times, philo->number, THINKING);
	philo->state = EATING;
}

void	philo_die(t_philo *philo, t_times *times)
{
	times->death = true;
	ft_print(*times, philo->number, DYING);
}

void	*p_routine(void *philo_in)
{
	t_philo	*philo;
	t_times	times;

	philo = (t_philo *)philo_in;
	times = philo->times;
	//printf("%i\n", philo->number);
	//while (ft_lock_sleep(&times) == true);
	if (philo->number % 2 == 0)
		go_sleeping(times.t_eat / 2, philo, &times); // Abhängig von den gegebenen Zeiten?
	while (times.death == false)
	{
		check_if_died(&times, philo);
		pthread_mutex_lock(&(times.mutex));
		if (times.death == true)
			printf("dead");
		pthread_mutex_unlock(&(times.mutex));
		times.philos[philo->number] = philo;
		if (times.meal_counter == times.meals_to_eat)
			break ;
		if (philo->state == SLEEPING)
			philo_sleep(philo, &times);
		if (philo->state == THINKING)
			philo_think(philo, &times);
		if (philo->state == EATING)
			philo_eat(philo, &times);
		if (philo->state == DYING)
			philo_die(philo, &times);
	}
	//check_if_died(&times, &philo);
	ft_stop(&times);
	return (NULL);
}

int	ft_lonely(t_times times)
{
	ft_print(times, 1, DYING);
	return (0);
}
