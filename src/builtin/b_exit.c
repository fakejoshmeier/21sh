/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:39:12 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/01 16:38:52 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern volatile	sig_atomic_t g_running;

void			b_exit(int ac, char **av, t_sh *sh)
{
	(void)av;
	(void)sh;
	if (ac != 1)
		ft_printf("usage: exit\n");
	else
		g_running = FALSE;
}
