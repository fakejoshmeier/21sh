/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 13:59:14 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/17 19:45:23 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

char	*get_path(int ac, char **av, t_sh *sh)
{
	t_cont		*tmp;

	if (ac == 1)
	{
		tmp = ft_map_get(&sh->env, ft_map_hash(&sh->env, "HOME"));
		return (tmp ? tmp->value : "/");
	}
	else if (ac == 2 && !ft_strcmp(av[1], "-"))
	{
		ft_printf("%s", sh->old);
		return (sh->old);
	}
	return (av[1]);
}

void	env_pwd(t_sh *sh)
{
	t_cont	*new;

	new = ft_map_remove(&sh->env, ft_map_hash(&sh->env, "PWD"));
	if (!new)
		new = (t_cont *)malloc(sizeof(t_cont));
	else
		free(new->var);
	new->var = ft_strnew(4 + ft_strlen(sh->cwd));
	new->value = new->var + 4;
	ft_strcat(new->var, "PWD=");
	ft_strcat(new->var, sh->cwd);
	ft_map_set(&sh->env, ft_map_hash(&sh->env, "PWD"), new);
}

void	b_cd(int ac, char **av, t_sh *sh)
{
	struct stat	dir;
	char		*path;

	path = get_path(ac, av, sh);
	if (ac > 2)
		ft_fprintf(STDERR_FILENO, "usage: cd [PATH]\n");
	else if (stat(path, &dir))
		ft_fprintf(STDERR_FILENO, "cd: no such file or directory: %s\n", path);
	else if (!S_ISDIR(dir.st_mode))
		ft_fprintf(STDERR_FILENO, "cd: not a directory: %s\n", path);
	else if (chdir(path))
		ft_fprintf(STDERR_FILENO, "cd: permission denied: %s\n", path);
	else
	{
		ft_strcpy(sh->old, sh->cwd);
		getcwd(sh->cwd, MAXPATHLEN);
		env_pwd(sh);
	}
}
