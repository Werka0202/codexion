/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wesobiec <wesobiec@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:15:14 by wesobiec          #+#    #+#             */
/*   Updated: 2026/07/06 15:34:26 by wesobiec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ar, char **av)
{
	t_sim	sim;

	if (ft_check_args(ar, av) != 0)
		return (1);
	if (ft_init_sim(av, &sim) != 0)
		return (1);
	start_coders(&sim);
	
	return(0);
}
