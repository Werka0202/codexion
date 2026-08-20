/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:04:22 by wesobiec          #+#    #+#             */
/*   Updated: 2026/08/20 14:04:34 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	enter_queue(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->queue_mutex);
	coder->sim->queue[coder->sim->wait_count] = coder->id;
	coder->sim->wait_count++;
	pthread_mutex_unlock(&coder->sim->queue_mutex);
}

int	is_my_turn(t_coder *coder)
{
	int		i;
	int		best;
	long	oldest;
	int		cur;

	if (coder->sim->scheduler == 0)
		return (coder->sim->queue[0] == coder->id);
	i = 0;
	best = coder->sim->queue[0];
	oldest = coder->sim->coders[best - 1].last_compile_start;
	while (i < coder->sim->wait_count)
	{
		cur = coder->sim->queue[i];
		if (coder->sim->coders[cur - 1].last_compile_start < oldest)
		{
			oldest = coder->sim->coders[cur - 1].last_compile_start;
			best = cur;
		}
		i++;
	}
	return (best == coder->id);
}

void	leave_queue(t_coder *coder)
{
	int	i;
	int	j;

	i = 0;
	pthread_mutex_lock(&coder->sim->queue_mutex);
	while (i < coder->sim->wait_count)
	{
		if (coder->sim->queue[i] == coder->id)
		{
			j = i;
			while (j < coder->sim->wait_count - 1)
			{
				coder->sim->queue[j] = coder->sim->queue[j + 1];
				j++;
			}
			coder->sim->wait_count--;
			break ;
		}
		i++;
	}
	pthread_cond_broadcast(&coder->sim->queue_cond);
	pthread_mutex_unlock(&coder->sim->queue_mutex);
}

void	wait_for_turn(t_coder *coder)
{
	enter_queue(coder);
	pthread_mutex_lock(&coder->sim->queue_mutex);
	while (!is_my_turn(coder))
		pthread_cond_wait(&coder->sim->queue_cond, &coder->sim->queue_mutex);
	pthread_mutex_unlock(&coder->sim->queue_mutex);
}
