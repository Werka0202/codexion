/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:51:09 by wesobiec          #+#    #+#             */
/*   Updated: 2026/07/06 15:35:30 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>

long	ft_get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("Error: gettimeofday failed\n");
		return (-1);
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_sleep(long time_in_ms)
{
	long	start_time;

	start_time = ft_get_time();
	while (ft_get_time() - start_time < time_in_ms)
		ft_sleep(100);
}
