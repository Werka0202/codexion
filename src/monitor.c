/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:51:23 by wesobiec          #+#    #+#             */
/*   Updated: 2026/08/20 13:41:22 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *monitor_routine(void *arg)
{
    t_sim *sim = (t_sim *)arg;
    int i;

    while (1)
    {
        i = 0;
        while (i < sim->num_coders)
        {
            pthread_mutex_lock(&sim->coders[i].time_mutex);
            
            if (ft_get_time() - sim->coders[i].last_compile_start >= sim->time_to_burnout)
            {
                pthread_mutex_unlock(&sim->coders[i].time_mutex);
                
                print_action(&sim->coders[i], "burned out");
                
                pthread_mutex_lock(&sim->dead_mutex);
                sim->is_dead = 1;
                pthread_mutex_unlock(&sim->dead_mutex);
                
                return (NULL);
            }
            pthread_mutex_unlock(&sim->coders[i].time_mutex);
            i++;
        }
        usleep(1000); 
    }
    return (NULL);
}