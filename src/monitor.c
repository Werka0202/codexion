/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:51:23 by wesobiec          #+#    #+#             */
/*   Updated: 2026/08/20 14:05:49 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_sim *sim, int i)
{
	if (ft_get_time() - sim->coders[i].last_compile_start >= sim->time_to_burnout)
	{
		pthread_mutex_unlock(&sim->coders[i].time_mutex);
		print_action(&sim->coders[i], "burned out");
		pthread_mutex_lock(&sim->dead_mutex);
		sim->is_dead = 1;
		pthread_mutex_unlock(&sim->dead_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	int		finished;

	sim = (t_sim *)arg;
	while (1)
	{
		i = 0;
		finished = 0;
		while (i < sim->num_coders)
		{
			pthread_mutex_lock(&sim->coders[i].time_mutex);
			if (check_burnout(sim, i))
				return (NULL);
			if (sim->req_compiles != -1
				&& sim->coders[i].compiles_count >= sim->req_compiles)
				finished++;
			pthread_mutex_unlock(&sim->coders[i].time_mutex);
			i++;
		}
		if (finished == sim->num_coders)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
