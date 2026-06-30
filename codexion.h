/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:08:15 by wesobiec          #+#    #+#             */
/*   Updated: 2026/06/19 21:03:55 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_sim	t_sim;

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;
	long			last_compile_start;
	int				compiles_count;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
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
	pthread_mutex_t	*dongles;
	t_coder			*coders;
}	t_sim;

int	ft_check_args(int ar, char **av);
int	ft_init_sim(char **av, t_sim *sim);
int	ft_alloc_init_sim(t_sim *sim);

#endif
