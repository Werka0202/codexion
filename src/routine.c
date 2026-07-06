/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:51:20 by wesobiec          #+#    #+#             */
/*   Updated: 2026/07/06 15:33:13 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("Coder %d started\n", coder->id);
	return (NULL);
}

int	start_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		if (pthread_create(&sim->coders[i].thread_id, NULL, routine, &sim->coders[i]) != 0)
		{
			printf("Error: pthread_create failed\n");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < sim->num_coders)
	{
		pthread_join(sim->coders[i].thread_id, NULL);
		i++;
	}
	return (0);
}