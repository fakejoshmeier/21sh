/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ast.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 13:46:02 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/03 22:36:00 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
** https://dev.to/oyagci/generating-a-parse-tree-from-a-shell-grammar-f1
**
** https://stackoverflow.com/questions/12942042/pipe-redirection-precedence
**
** Run through list of tokens, creating a new leaf and ripping off the
** remainder of the token list, passing it into the leaf to be parsed.  Done
** through the magic of recursion!
*/

int		next_op(t_tkn *token, int op)
{
	t_tkn	*tmp;

	tmp = token->next;
	if (op == REDIRECT)
	{
		if (token->type == IONUMBER)
			tmp = tmp->next;
		while (tmp)
		{
			if (tmp->op_type >= LESS && tmp->op_type <= CLOBBER)
				return (1);
			tmp = tmp->next;
		}
	}
	while (tmp)
	{
		if (tmp->op_type == op)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

t_ast	*create_redirect(t_tkn **token)
{
	t_ast	*root;

	root = create_leaf(token, OPERATOR);
	if (((*token)->op_type >= LESS && (*token)->op_type <= CLOBBER) ||
		(*token)->type == IONUMBER)
	{
		if (next_op(*token, REDIRECT))
		{
			while (((*token)->op_type >= LESS && (*token)->op_type <= CLOBBER)
				|| (*token)->type == IONUMBER)
				root = create_node(root, create_leaf(token, WORD),
				create_leaf(token, OPERATOR));
		}
		else
		{
			root = create_node(root, create_leaf(token, WORD),
			create_leaf(token, OPERATOR));
		}
	}
	return (root);
}

/*
** Creates splits based on pipes
*/

t_ast	*create_pipeline(t_tkn **token)
{
	t_ast	*root;

	root = create_redirect(token);
	if ((*token)->op_type == PIPE)
	{
		if (next_op(*token, PIPE))
		{
			while ((*token)->op_type == PIPE)
				root = create_node(root, create_leaf(token, WORD),
				create_redirect(token));
		}
		else
			root = create_node(root, create_leaf(token, WORD),
			create_redirect(token));
	}
	return (root);
}

/*
** Creates splits in the tree based on '||' or '&&'
*/

t_ast	*create_list(t_tkn **token)
{
	t_ast	*root;

	root = create_pipeline(token);
	if ((*token)->op_type == ANDIF || (*token)->op_type == ORIF)
	{
		if (next_op(*token, ANDIF) || next_op(*token, ORIF))
		{
			while ((*token)->op_type == ANDIF || (*token)->op_type == ORIF)
				root = create_node(root, create_leaf(token, WORD),
				create_pipeline(token));
		}
		else
			root = create_node(root, create_leaf(token, WORD),
			create_pipeline(token));
	}
	return (root);
}

/*
** These functions will run through the tokens until the next operator, and
** separate nodes based on pipes, certain operators, etc.
** This level will split based on separators, i.e. ';' and '&'.
*/

t_ast	*create_ast(t_tkn **token)
{
	t_ast	*root;

	root = create_list(token);
	if ((*token)->op_type == SEPARATOR)
	{
		if (next_op(*token, SEPARATOR))
		{
			while ((*token)->op_type == SEPARATOR)
				root = create_node(root, create_leaf(token, WORD),
				create_list(token));
		}
		else
			root = create_node(root, create_leaf(token, WORD),
			create_list(token));
	}
	if ((*token)->type == 404)
		token_del(token);
	return (root);
}
