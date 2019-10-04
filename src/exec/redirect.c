/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/03 11:39:38 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/03 21:23:30 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
** For redirections into an FD, check the first token in ast->left.  If it's a
** number, it's actually an IO number and I'll work it accordingly.
*/

int		exec_redirect(t_ast *ast, t_sh *sh)
{
	int		fd;
	t_tkn	*tmp;

	if (ast->type == IONUMBER)
	{
		fd = ft_atoi(ast->token->val);
		tmp = ast->token;
		ast->token = tmp->next;
		token_del(&tmp);
	}
	else
		fd = 1;
	(void)sh;
	return (fd);
}