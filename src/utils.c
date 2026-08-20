/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:51:17 by wesobiec          #+#    #+#             */
/*   Updated: 2026/08/20 19:09:31 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_action(t_coder *coder, char *action)
{
	long	current_time;

	pthread_mutex_lock(&coder->sim->print_mutex);
	if (coder->sim->is_dead == 0)
	{
		current_time = ft_get_time() - coder->sim->start_time;
		printf("%ld %d %s\n", current_time, coder->id, action);
	}
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
