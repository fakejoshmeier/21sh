/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ast_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 16:29:04 by jmeier            #+#    #+#             */
/*   Updated: 2019/09/17 21:01:55 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "sh.h"

/*
** AST related functions.  Adding nodes, handling operator precedence according
** to shell grammar
*/

/*
** On a given leaf, rip out chunks of the linked list that is my tokens list.
*/


t_ast	*create_leaf(t_tkn **token, int delim)
{
	t_ast	*new;
	t_tkn	*tmp;
	t_tkn	*ret;

	if (!(*token) || (*token)->type == 404)
		return (NULL);
	new = ft_memalloc(sizeof(t_ast));
	new->parent = NULL;
	new->left = NULL;
	new->right = NULL;
	new->type = (*token)->type;
	new->op_type = (*token)->op_type;
	new->token = *token;
	ft_printf("TOKEN - %s\nTYPE - %i\nOPERATION - %i\n\n", new->token->val, new->type, new->op_type);
	ret = *token;
	while (ret->next && ret->type != delim)
	{
		tmp = ret;
		ret = tmp->next;
	}
	ft_printf("TOKEN - %s\nTYPE - %i\nOPERATION - %i\n-----------------------------\n", ret->val, ret->type, ret->op_type);
	*token = ret;
	tmp->next = NULL;
	return (new);
}

t_ast	*create_node(t_ast *left, t_ast *node, t_ast *right)
{
	if (!node)
		return (left);
	node->left = left;
	node->right = right;
	if (left)
		left->parent = node;
	if (right)
		right->parent = node;
	return (node);
}

void	ast_del(t_ast **ast)
{
	if (!*ast)
		return ;
	ast_del(&(*ast)->left);
	ast_del(&(*ast)->right);
	free((*ast)->token);
	free(*ast);
	*ast = NULL;
}
