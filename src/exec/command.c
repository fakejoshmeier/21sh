/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 14:38:55 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/03 15:47:56 by jmeier           ###   ########.fr       */
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

int			exec_command(t_ast *ast, t_sh *s)
{
	t_fptr	b_in;
	char	**av;
	char	*sys;
	int		ac;

	av = token_to_array(ast->token, s);
	if (!av)
		return (0);
	ac = 0;
	while (av[ac])
		++ac;
	ft_strtolower(&av[0]);
	if (check_executable(av[0]))
		execute(av[0], av, s);
	else if ((b_in = ft_map_get(&s->builtin, ft_map_hash(&s->builtin, av[0]))))
		b_in(ac, av, s);
	else if ((sys = ft_map_get(&s->path, ft_map_hash(&s->path, av[0]))))
		execute(sys, av, s);
	else
	{
		ERROR_PROMPT(COMMAND_ERR, av[0]);
		return (0);
	}
	ft_arraydel(&av);
	return (1);
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
		exit(1);
	}
	else
		waitpid(pid, 0, 0);
	enter_raw_mode();
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
