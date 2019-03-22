/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_setenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 13:59:35 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/08 06:28:05 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	b_setenv(int ac, char **av, t_sh *sh)
{
	t_cont	*new;

	if (ac != 3)
	{
		ac == 1 ? b_env(ac, av, sh) : ft_printf("usage: setenv VAR [VALUE]\n");
		return ;
	}
	new = ft_map_remove(&sh->env, ft_map_hash(&sh->env, av[1]));
	if (!new)
		new = (t_cont *)malloc(sizeof(t_cont));
	else
		free(new->var);
	new->var = ft_strnew(ft_strlen(av[1]) + ft_strlen(av[2] + 1));
	new->value = new->var + ft_strlen(av[1]) + 1;
	ft_strcat(new->var, av[1]);
	ft_strcat(new->var, "=");
	ft_strcat(new->var, av[2]);
	ft_map_set(&sh->env, ft_map_hash(&sh->env, av[1]), new);
	if (ac == 3 && !ft_strcmp(av[1], "PATH"))
	{
		ft_map_clean(&sh->path, free);
		update_path(sh);
	}
}
