/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 20:29:23 by wesobiec          #+#    #+#             */
/*   Updated: 2026/08/20 14:04:55 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_check_args(int ar, char **av)
{
	int	i;
	int	j;

	j = 1;
	if (ar != 9)
		return (printf("Error: Wrong numbers of arguments!\n"), 1);
	while (j < ar - 1)
	{
		i = 0;
		while (av[j][i])
		{
			if (av[j][i] < '0' || av[j][i] > '9')
				return (printf("Error: arguments must be numbers\n"), 1);
			i++;
		}
		j++;
	}
	return (0);
}

int	ft_init_sim(char **av, t_sim *sim)
{
	sim->num_coders = atoi(av[1]);
	sim->time_to_burnout = atoi(av[2]);
	sim->time_to_compile = atoi(av[3]);
	sim->time_to_debug = atoi(av[4]);
	sim->time_to_refactor = atoi(av[5]);
	sim->req_compiles = atoi(av[6]);
	sim->cooldown = atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		sim->scheduler = 0;
	else if (strcmp(av[8], "edf") == 0)
		sim->scheduler = 1;
	else
		return (printf("Error: last argument must be edf or fifo\n"), 1);
	return (ft_alloc_init_sim(sim));
}

void	ft_init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].last_compile_start = 0;
		pthread_mutex_init(&sim->coders[i].time_mutex, NULL);
		sim->coders[i].compiles_count = 0;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % sim->num_coders];
		sim->coders[i].sim = sim;
		i++;
	}
}

int	ft_alloc_init_sim(t_sim *sim)
{
	int	i;

	i = 0;
	sim->coders = malloc(sizeof(t_coder) * sim->num_coders);
	sim->dongles = malloc(sizeof(t_dongle) * sim->num_coders);
	sim->queue = malloc(sizeof(int) * sim->num_coders);
	if (!sim->coders || !sim->dongles || !sim->queue)
		return (1);
	while (i < sim->num_coders)
	{
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		sim->dongles[i].last_released = 0;
		i++;
	}
	ft_init_coders(sim);
	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->dead_mutex, NULL);
	pthread_mutex_init(&sim->queue_mutex, NULL);
	pthread_cond_init(&sim->queue_cond, NULL);
	sim->wait_count = 0;
	sim->is_dead = 0;
	return (0);
}
