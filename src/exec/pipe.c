/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 17:26:55 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/01 15:50:53 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

void	exit_on_error(char *str)
{
	ERROR_PROMPT("fart", str);
	exit(1);
}

// static int	ft_launch_one_side(t_ast *side)
// {
// 	int		save[3];
// 	char	**cmd;
// 	char	*path;
// 	int		status;

// 	if ((status = ft_init_launch(save, side)) == REDIR_ERROR)
// 		return (status);
// 	if ((cmd = ft_cmd_into_tab(side)))
// 	{
// 		if (ft_is_builtin(cmd[0]))
// 			status = ft_launch_builtin(cmd);
// 		else if ((status = ft_get_path(cmd[0], &path)) == PATH_OK)
// 		{
// 			if ((status = execve(path, cmd, g_shell->env)) == -1)
// 				ft_exit(STR_EXECVE_ERROR, 1);
// 			free(path);
// 		}
// 		else
// 			return (status);
// 		ft_freetab(&cmd);
// 	}
// 	else
// 		return (EXIT_SUCCESS);
// 	ft_restore_std_fd(side, save);
// 	return (status);
// }

// static int	ft_pipe_to_right(int fd[2], t_ast *right)
// {
// 	pid_t	pid_right;
// 	int		status_right;

// 	if ((pid_right = fork()) == -1)
// 		exit_on_error("fork failed");
// 	if (pid_right == 0)
// 	{
// 		close(fd[1]);
// 		ft_make_dup2(right->token->val, fd[0], STDIN_FILENO);
// 		if (right->parent->parent && right->parent->parent->op_type == PIPE)
// 			exit(launch_pipeline(right, right->parent->parent->right));
// 		else
// 			exit(launch_one_side(right));
// 	}
// 	else
// 	{
// 		close(fd[1]);
// 		waitpid(pid_right, &status_right, 0);
// 	}
// 	return (ft_exit_status(status_right));
// }

// int			launch_pipeline(t_ast *left, t_ast *right)
// {
// 	int		fd[2];
// 	pid_t	pid_left;
// 	int		status_right;

// 	if (pipe(fd) == -1)
// 	{
// 		ft_printf(BLU"jo.sh"RES": %s: pipe error\n", left->left->token->val);
// 		return (0);
// 	}
// 	pid_left = fork();
// 	if (pid_left == -1)
// 		exit_on_error("fork failed");
// 	else if (pid_left == 0)
// 	{
// 		close(fd[0]);
// 		ft_make_dup2(left->token->val, fd[1], STDOUT_FILENO);
// 		exit(ft_launch_one_side(left));
// 	}
// 	else
// 	{
// 		status_right = pipe_right(right);
// 		close(fd[0]);
// 		waitpid(pid_left, NULL, 0);
// 	}
// 	return (status_right);
// }

int		exec_pipe(t_ast *ast, t_sh *sh)
{
	// if (!ast->left)
	// 	return (launch_pipeline(ast, ast->parent->right));
	// else
	if (ast->left)
		return (exec_pipe(ast->left, sh));
	else
		return (0);
}
