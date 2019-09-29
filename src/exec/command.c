/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 14:38:55 by jmeier            #+#    #+#             */
/*   Updated: 2019/09/28 15:05:14 by jmeier           ###   ########.fr       */
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

int		exec_command(t_ast *ast, t_sh *sh)
{
	t_fptr	b_in;
	char	*sys;
	char	**e;

	tmp = ft_strsplit(expand(ast->token->val, sh), ' ');
	if (check_executable(e[0]))
		execute(e[0], e, sh);
	else if ((b_in = ft_map_get(&sh->builtin, ft_map_hash(&sh->builtin, e[0]))))
		b_in(ast->next->size, ast->next->arr, sh);
	else if ((sys = ft_map_get(&sh->path, ft_map_hash(&sh->path, e[0]))))
		execute(sys, e, sh);
	else
	{
		ft_printf(BLU"jo.sh"RES": command not found: %s\n", tmp[0]);
		return (0);
	}
	ft_arraydel(tmp);
	return (1);
}

void	execute(char *cmd, char **av, t_sh *sh)
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
		ft_printf("%i\n", errno);
		ft_putendl("Failed to execute command");
		ft_arraydel(av);
		exit(1);
	}
	else
		waitpid(pid, 0, 0);
	enter_raw_mode();
	ft_arraydel(env);
}

int		check_executable(char *exe)
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
