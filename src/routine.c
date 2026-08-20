/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:51:20 by wesobiec          #+#    #+#             */
/*   Updated: 2026/08/20 14:05:26 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	grab_one(t_coder *coder, t_dongle *dongle)
{
	long	elapsed;

	pthread_mutex_lock(&dongle->mutex);
	elapsed = ft_get_time() - dongle->last_released;
	if (elapsed < coder->sim->cooldown)
		ft_sleep(coder->sim->cooldown - elapsed);
	print_action(coder, "has taken a dongle");
}

void	take_dongles(t_coder *coder)
{
	wait_for_turn(coder);
	if (coder->id == coder->sim->num_coders)
	{
		grab_one(coder, coder->right_dongle);
		grab_one(coder, coder->left_dongle);
	}
	else
	{
		grab_one(coder, coder->left_dongle);
		grab_one(coder, coder->right_dongle);
	}
	leave_queue(coder);
}

void	*routine(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	while (c->sim->req_compiles == -1 || c->compiles_count < c->sim->req_compiles)
	{
		take_dongles(c);
		print_action(c, "is compiling");
		pthread_mutex_lock(&c->time_mutex);
		c->last_compile_start = ft_get_time();
		pthread_mutex_unlock(&c->time_mutex);
		ft_sleep(c->sim->time_to_compile);
		c->compiles_count++;
		c->left_dongle->last_released = ft_get_time();
		pthread_mutex_unlock(&c->left_dongle->mutex);
		c->right_dongle->last_released = ft_get_time();
		pthread_mutex_unlock(&c->right_dongle->mutex);
		print_action(c, "is debugging");
		ft_sleep(c->sim->time_to_debug);
		print_action(c, "is refactoring");
		ft_sleep(c->sim->time_to_refactor);
	}
	return (NULL);
}

int	start_coders(t_sim *sim)
{
	int			i;
	pthread_t	monitor_thread;

	sim->start_time = ft_get_time();
	i = 0;
	while (i < sim->num_coders)
	{
		sim->coders[i].last_compile_start = sim->start_time;
		if (pthread_create(&sim->coders[i].thread_id, NULL, routine, &sim->coders[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, sim) != 0)
		return (1);
	i = 0;
	while (i < sim->num_coders)
		pthread_join(sim->coders[i++].thread_id, NULL);
	pthread_join(monitor_thread, NULL);
	return (0);
}
