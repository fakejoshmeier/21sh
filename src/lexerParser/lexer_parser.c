/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 23:20:29 by jmeier            #+#    #+#             */
/*   Updated: 2019/09/24 17:37:53 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

/*
** Adds command to history, even in the case of invalid commands, as zsh does.
** After command is added, the input is separated into tokens, then the word
** tokens are stored into separate arrays, separated by their operators.
** If there are parse errors, the execution stops and everything should be
** cleaned up.  For redirections, pipes, and other things that require
** two processes, there is a specialized execution set, otherwise it all
** continues as did before.
*/

//https://dev.to/oyagci/generating-a-parse-tree-from-a-shell-grammar-f1

/*
** Follow shell grammar hierarchy
*/

int		exec_separator(t_ast *ast)
{
	ast_redirect(ast->left);
	return (ast_redirect(ast->right));
}

int		exec_andif(t_ast *ast)
{
	if (ast_redirect(ast->left))
		return (ast_redirect(ast->right));
	else
		return (1);
}

int		exec_orif(t_ast *ast)
{
	if (ast_redirect(ast->left))
		return (1);
	else
		return (ast_redirect(ast->right));
}

int		ast_redirect(t_ast *ast, t_sh *sh)
{
	if (!ast)
		return (0);
	if (ast->op_type == SEPARATOR)
		return (exec_separator(ast));
	else if (ast->op_type == ANDIF)
		return (exec_andif(ast));
	else if (ast->op_type == ORIF)
		return (exec_orif(ast));
	else if (ast->op_type == PIPE)
		return (exec_pipe(ast));
	else if (ast->type != OPERATOR)
		return (exec_command(ast));
	return (1);
}

void	lexer_parser(t_line *line, t_sh *sh)
{
	t_tkn	*tokens;
	t_ast	*ast;

	sh->history = !sh->history ? hist_new((char *)line->data) :
		hist_add(sh->history, (char *)line->data);
	sh->curr = NULL;
	// tokens = lexer(expand((char *)line->data, sh));
	tokens = lexer((char *)line->data);
	line->length = 0;
	ast = create_ast(&tokens);
	ast_redirect(ast, sh);
	if (!ast)
		ast_del(&ast);
}
