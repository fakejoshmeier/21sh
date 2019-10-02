/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 15:53:35 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/01 15:58:57 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

static void	fork_error(void)
{
	ft_printf(BLU"jo.sh"RES": fork failed\n");
	exit(1);
}

int			pipe_right(int fd[2], t_ast *down, t_ast *ast)
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) == -1)
		fork_error();
	if (!pid)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		exit(ft_pi)
	}
	else
	{
		close(fd[1]);
		waitpid(pid, &status, 0);
	}
	exit(status);
}

int		ft_pipe(t_ast *ast, t_ast *down, t_sh *sh)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		ft_printf(BLU"jo.sh"RES": pipe failed\n");
	else
	{
		if ((pid = fork()) == -1)
			fork_error();
		else if (!pid)
		{
			close(fd[0]);
			dup2(fd[0], STDOUT_FILENO) ? 0 :
				ft_printf(BLU"jo.sh"RES": dup2 failed\n");
			exit(run_one());
		}
		else
		{
			status = pipe_to_right(fd, down);
			close(fd[0]);
			waitpid(pid, NULL, 0);
		}
	}
	return (status);
}