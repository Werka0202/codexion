/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:08:15 by wesobiec          #+#    #+#             */
/*   Updated: 2026/08/20 19:06:31 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_sim	t_sim;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long			last_released;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;
	long			last_compile_start;
	pthread_mutex_t	time_mutex;
	int				compiles_count;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_sim			*sim;
}	t_coder;

typedef struct s_sim
{
	int				num_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				req_compiles;
	long			cooldown;
	int				scheduler;
	long			start_time;
	int				is_dead;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	queue_cond;
	int				*queue;
	int				wait_count;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_sim;

int		ft_check_args(int ar, char **av);
int		ft_init_sim(char **av, t_sim *sim);
int		ft_alloc_init_sim(t_sim *sim);
int		start_coders(t_sim *sim);
long	ft_get_time(void);
void	ft_sleep(long time_in_ms);
void	print_action(t_coder *coder, char *action);
void	take_dongles(t_coder *coder);
void	*monitor_routine(void *arg);
void	wait_for_turn(t_coder *coder);
void	leave_queue(t_coder *coder);

#endif