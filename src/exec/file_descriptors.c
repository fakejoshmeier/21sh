/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_descriptors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/03 11:39:38 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/17 22:40:24 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
** For redirections into an FD, check the first token in ast->left.  If it's a
** number, it's actually an IO number and I'll work it accordingly.
**
** https://www.gnu.org/software/bash/manual/html_node/Redirections.html
**
** It makes sense to just handle all the fd related stuff here.
**
** I need to tcsetattr with STDERR
*/

int		fd_aggregate(t_tkn *token)
{
	int		dst_fd;
	int		src_fd;

	src_fd = token->op_type == LESSAND ? STDIN_FILENO : STDOUT_FILENO;
	if (token->op_type == IONUMBER && token->next)
	{
		if (token->next->type == REDIRECT)
			return (fd_redirect(token));
		src_fd = ft_atoi(token->val);
		token = token->next;
	}
	dst_fd = ft_atoi(token->next->val);
	ft_printf("%i\n", dst_fd);
	if (token->next->type == IONUMBER)
		return (fd_dup2(token->next->val, dst_fd, src_fd));
	else if (ft_strequ(token->next->val, "-"))
		return (close(src_fd));
	else
		return (redirect_error(1));
		// return (redirect_error(token->next->val, STR_AMB_REDIR));
}

int		fd_dup2(char *str, int dst_fd, int src_fd)
{
	if ((dup2(dst_fd, src_fd)) != -1)
		return (0);
	else
	{
		if (errno == EBADF)
			ERR_P(BAD_FD_ERR, str);
		else
			ERR_P(DUP_ERR, str);
		return (1);
	}
}

int		fd_redirect(t_tkn *token)
{
	int		src_fd;

	errno = 0;
	if (token->type == IONUMBER)
	{
		src_fd = ft_atoi(token->val);
		token = token->next;
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
	return (fd_dup2(token->next->val, token->fd, src_fd));
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
	tmp = ast->token;
	if (!tmp)
		return (1);
	res = 0;
	while (tmp)
	{
		if (tmp->type == AGGREGATE || tmp->type == IONUMBER)
			res = fd_aggregate(tmp);
		else if (tmp->op_type == D_LESS)
			res = fd_heredoc(tmp, sh);
		else if (tmp->type == REDIRECT)
			res = fd_redirect(tmp);
		if (res != 0)
			return (1);
		tmp = tmp->next;
	}
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
