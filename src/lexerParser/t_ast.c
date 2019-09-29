/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ast.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 13:46:02 by jmeier            #+#    #+#             */
/*   Updated: 2019/09/17 21:03:11 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

/*
** Run through list of tokens, creating a new leaf and ripping off the
** remainder of the token list, passing it into the leaf to be parsed.  Done
** through the magic of recursion!
*/

int		next_op(t_tkn *token, int op)
{
	t_tkn	*tmp;

	tmp = token->next;
	while (tmp)
	{
		if (tmp->op_type == op)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/*
** Creates splits based on pipes
*/

t_ast	*create_pipeline(t_tkn **token)
{
	t_ast	*root;

	root = create_leaf(token, OPERATOR);
	if ((*token)->op_type == PIPE)
	{
		if (next_op(*token, PIPE))
		{
			while ((*token)->op_type == PIPE)
			{
				root = create_node(root, create_leaf(token, WORD),
				create_leaf(token, OPERATOR));
			}
		}
		else
			root = create_node(root, create_leaf(token, WORD), 
			create_leaf(token, OPERATOR)); 
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
		{
			root = create_node(root, create_leaf(token, WORD),
			create_list(token));
		}
	}
	return (root);
}
