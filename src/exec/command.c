/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 14:38:55 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/10 23:05:36 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

/*
** The first call checks for compiled executables, eg this.
** The second call is builtin functions
** The final call is system functions like ls.
**
** Great resource: http://www.sarathlakshman.com/2012/09/24/implementation-
** overview-of-redirection-and-pipe-operators-in-shell
*/

char		**token_to_array(t_tkn *token)
{
	t_tkn	*tmp;
	char	**ret;

	ret = NULL;
	if (!token)
		return (NULL);
	tmp = token;
	while (tmp)
	{
		if (tmp->type == IO_NUMBER)
			tmp = tmp->next;
		else if (tmp->type == REDIRECT)
		{
			if (tmp->next->type == IO_NUMBER)
				tmp = tmp->next;
			tmp = tmp->next->next;
		}
		else
		{
			ret = ft_arr_add(ret, tmp->val);
			tmp = tmp->next;
		}
	}
	return (ret);
}

int			exec_command(t_ast *ast, t_sh *s)
{
	t_fptr	b_in;
	char	**av;
	char	*sys;

	if (!(av = token_to_array(ast->token)))
		return (1);
	if (set_fd(ast, s))
		return (1);
	ft_strtolower(&av[0]);
	if (check_executable(av[0]))
		execute(av[0], av, s);
	else if ((b_in = ft_map_get(&s->builtin, ft_map_hash(&s->builtin, av[0]))))
		b_in(ft_arrlen(av), av, s);
	else if ((sys = ft_map_get(&s->path, ft_map_hash(&s->path, av[0]))))
		execute(sys, av, s);
	else
	{
		ERROR_PROMPT(COMMAND_ERR, av[0]);
		return (1);
	}
	unset_fd(ast, s);
	ft_arraydel(&av);
	return (0);
}

void		execute(char *cmd, char **av, t_sh *sh)
{
	pid_t	pid;
	char	**env;

	pid = fork();
	env = NULL;
	if (pid == 0)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &sh->term_settings);
		env = map_to_array(&sh->env);
		execve(cmd, av, env);
		ERROR_PROMPT(COMMAND_FAIL, cmd);
		ft_arraydel(&av);
		exit(STDIN_FILENO);
	}
	else
		waitpid(pid, 0, 0);
	ft_arraydel(&env);
}

int			check_executable(char *exe)
{
	struct stat	info;

	if (!ft_strcmp(dirname(exe), ".") && ft_strncmp(exe, ".", 1))
		return (0);
	if (access(exe, X_OK))
		return (0);
	stat(exe, &info);
	if (!S_ISREG(info.st_mode))
		return (0);
	return (1);
}
