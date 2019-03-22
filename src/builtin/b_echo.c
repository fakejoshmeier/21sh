/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 14:24:25 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/07 01:00:36 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	b_echo(int ac, char **av, t_sh *sh)
{
	int		i;
	int		nl;

	(void)sh;
	i = 0;
	nl = ac > 1 && av[1][0] == '-' && av[1][1] == 'n' ? 1 : 0;
	while (++i < ac)
	{
		if (!av[i] || (ft_strlen(av[i]) == 1 && ft_isspace(av[i][0])))
			continue ;
		ft_printf("%s", av[i]);
		if (i + 1 < ac)
			ft_putchar(' ');
	}
	if (!nl)
		ft_putchar('\n');
}
