/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:51:20 by wesobiec          #+#    #+#             */
/*   Updated: 2026/08/20 13:48:09 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void take_dongles(t_coder *coder)
{
	if (coder->id == coder->sim->num_coders)
    {
        pthread_mutex_lock(coder->right_dongle);
        print_action(coder, "has taken a dongle");
        pthread_mutex_lock(coder->left_dongle);
        print_action(coder, "has taken a dongle");
    }
	else
    {
        pthread_mutex_lock(coder->left_dongle);
        print_action(coder, "has taken a dongle");
        pthread_mutex_lock(coder->right_dongle);
        print_action(coder, "has taken a dongle");
    }
}

void *routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	
	while	(coder->sim->req_compiles == -1 || coder->compiles_count < coder->sim->req_compiles)
	{
		take_dongles(coder);
		print_action(coder, "is compiling");
		pthread_mutex_lock(&coder->time_mutex);
		coder->last_compile_start = ft_get_time();
		pthread_mutex_unlock(&coder->time_mutex);
		ft_sleep(coder->sim->time_to_compile);
		coder->compiles_count++;
		pthread_mutex_unlock(coder->left_dongle);
		pthread_mutex_unlock(coder->right_dongle);
		print_action(coder, "is debugging");
		ft_sleep(coder->sim->time_to_debug);
		print_action(coder, "is refactoring");
		ft_sleep(coder->sim->time_to_refactor);
	}
	return (NULL);
}

int start_coders(t_sim *sim)
{
    int         i;
    pthread_t   monitor_thread;

    sim->start_time = ft_get_time(); 

    i = 0;
    while (i < sim->num_coders)
    {
		sim->coders[i].last_compile_start = sim->start_time;
		
        if (pthread_create(&sim->coders[i].thread_id, NULL, routine, &sim->coders[i]) != 0)
        {
            printf("Error: pthread_create failed\n");
            return (1);
        }
        i++;
    }
    
    if (pthread_create(&monitor_thread, NULL, monitor_routine, sim) != 0)
    {
        printf("Error: Monitor thread failed\n");
        return (1);
    }

    i = 0;
    while (i < sim->num_coders)
    {
        pthread_join(sim->coders[i].thread_id, NULL);
        i++;
    }
    
    pthread_join(monitor_thread, NULL);
    return (0);
}
