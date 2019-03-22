/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_unsetenv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 13:59:27 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/08 06:29:13 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	b_unsetenv(int ac, char **av, t_sh *sh)
{
	t_cont	*old;

	if (ac != 2)
	{
		ft_printf("usage: unsetenv VAR");
		return ;
	}
	old = ft_map_remove(&sh->env, ft_map_hash(&sh->env, av[1]));
	if (!old)
		return ;
	free(old->var);
	free(old);
	if (!ft_strcmp(av[1], "PATH"))
		ft_map_clean(&sh->path, free);
}
