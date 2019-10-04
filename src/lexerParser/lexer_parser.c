/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 23:20:29 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/03 22:05:12 by jmeier           ###   ########.fr       */
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
**
** https://unix.stackexchange.com/questions/159513/what-are-the-shells-control-
** and-redirection-operators
*/

//https://dev.to/oyagci/generating-a-parse-tree-from-a-shell-grammar-f1

/*
** Follow shell grammar hierarchy
*/

int		exec_separator(t_ast *ast, t_sh *sh)
{
	if (ast->token->val[0] == '&')
		ast_redirect(ast->left, sh);
	else
		ast_redirect(ast->left, sh);
	return (ast_redirect(ast->right, sh));
}

int		exec_andif(t_ast *ast, t_sh *sh)
{
	if (ast_redirect(ast->left, sh))
		return (ast_redirect(ast->right, sh));
	else
		return (1);
}

int		exec_orif(t_ast *ast, t_sh *sh)
{
	if (ast_redirect(ast->left, sh))
		return (1);
	else
		return (ast_redirect(ast->right, sh));
}

int		ast_redirect(t_ast *ast, t_sh *sh)
{
	if (!ast)
		return (0);
	if (ast->op_type == SEPARATOR)
		return (exec_separator(ast, sh));
	else if (ast->op_type == ANDIF)
		return (exec_andif(ast, sh));
	else if (ast->op_type == ORIF)
		return (exec_orif(ast, sh));
	else if (ast->op_type == PIPE)
		return (exec_pipe(ast, sh));
	else if ((ast->type == OPERATOR && ast->token->op_type >= LESS &&
		ast->token->op_type <= CLOBBER) || ast->type == IONUMBER)
		return (exec_redirect(ast, sh));
	else if (ast->type != OPERATOR)
		return (exec_command(ast, sh));
	return (1);
}

void	lexer_parser(t_line *line, t_sh *sh)
{
	t_tkn	*tokens;
	t_ast	*ast;

	sh->history = !sh->history ? hist_new((char *)line->data) :
		hist_add(sh->history, (char *)line->data);
	sh->curr = NULL;
	tokens = lexer((char *)line->data);
	line->length = 0;
	t_tkn *tmp = tokens;
	while (tmp)
	{
		ft_printf("%i - %i - %s\n", tmp->type, tmp->op_type, tmp->val);
		tmp = tmp->next;
	}
	ast = create_ast(&tokens);
	if (ast)
	{
		ft_print_ast(ast, "HEAD", 0);
		// ast_redirect(ast, sh);
		ast_del(&ast);
	}
}
