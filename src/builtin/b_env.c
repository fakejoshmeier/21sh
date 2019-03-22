/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 22:04:07 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/08 16:12:07 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	b_env(int ac, char **av, t_sh *sh)
{
	char		**env;
	unsigned	i;

	(void)av;
	if (ac > 1)
	{
		ft_printf("usage: env [-s]\n");
		return ;
	}
	env = map_to_array(&sh->env);
	i = 0;
	while (i < sh->env.count)
	{
		ft_printf("%s\n", env[i]);
		free(env[i++]);
	}
	free(env);
}
