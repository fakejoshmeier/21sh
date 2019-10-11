/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/03 11:39:38 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/10 22:12:28 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
** For redirections into an FD, check the first token in ast->left.  If it's a
** number, it's actually an IO number and I'll work it accordingly.
**
** https://www.gnu.org/software/bash/manual/html_node/Redirections.html
*/

int		make_dup2(char *str, int dst_fd, int src_fd)
{
	if ((dup2(dst_fd, src_fd)) != -1)
		return (0);
	else
	{
		if (errno == EBADF)
			ERROR_PROMPT(BAD_FD_ERR, str);
		else
			ERROR_PROMPT(DUP_ERR, str);
		return (1);
	}
}
// Work on the IO Number stuff later on
int		fd_redirect(t_tkn *token)
{
	int		src_fd;
	t_tkn	*tmp;

	errno = 0;
	if (token->type == IONUMBER)
	{
		src_fd = ft_atoi(token->val);
		tmp = token;
		token = tmp->next;
		tmp->next = NULL;
		token_del(&tmp);
	}
	else
		src_fd = token->op_type == LESS ? STDIN_FILENO : STDOUT_FILENO;
	if (token->op_type == D_GREAT)
		token->fd = open(token->next->val, APPEND, PERMISSIONS);
	else if (token->op_type == GREAT || token->op_type == CLOBBER)
		token->fd = open(token->next->val, TRUNCATE, PERMISSIONS);
	else
		token->fd = open(token->next->val, O_RDONLY);
	if (token->fd == -1)
		return (redirect_error(errno));
	return (make_dup2(token->next->val, token->fd, src_fd));
}

/*
** Use the IO Numbers and redirection operators to figure out how I'm going to
** write my results.  Once the results are written to the chosen FD, then I'll
** reset to the proper raw mode with the saved fds in sh in unset, closing any
** and all fds I may have left open.
*/

int		set_fd(t_ast *ast, t_sh *sh)
{
	t_tkn	*tmp;
	int		res;

	sh->fds[0] = dup(STDIN_FILENO);
	sh->fds[1] = dup(STDOUT_FILENO);
	sh->fds[2] = dup(STDERR_FILENO);
	res = 0;
	tmp = ast->token;
	while (tmp)
	{
		if (tmp->type == REDIRECT || tmp->type == IO_NUMBER)
		{
			if (tmp->op_type == LESSAND || tmp->op_type == GREATAND)
				res = fd_aggregate(tmp, sh);
			else if (tmp->op_type == D_LESS)
				res = fd_heredoc(tmp, sh);
			else
				res = fd_redirect(tmp);
			if (res != 0)
				return (1);
		}
		tmp = tmp->next;
	}
	enter_raw_mode();
	return (0);
}

void	unset_fd(t_ast *ast, t_sh *sh)
{
	t_tkn *tmp;

	dup2(sh->fds[0], STDIN_FILENO);
	dup2(sh->fds[1], STDOUT_FILENO);
	dup2(sh->fds[2], STDERR_FILENO);
	close(sh->fds[0]);
	close(sh->fds[1]);
	close(sh->fds[2]);
	if (!ast)
		return ;
	tmp = ast->token;
	while (tmp)
	{
		if (tmp->type == REDIRECT)
		{
			if (tmp->fd != -1)
				close(tmp->fd);
		}
		tmp = tmp->next;
	}
	enter_raw_mode();
}
