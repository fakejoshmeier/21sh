/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 22:28:40 by jmeier            #+#    #+#             */
/*   Updated: 2019/06/15 10:03:03 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

void	bin_helper(t_list **list)
{
	t_list	*tmp;

	if (!list || !(*list))
		return ;
	tmp = *list;
	free(tmp->content);
	tmp->content = NULL;
	free(tmp);
	tmp = NULL;
}

void	free_token(t_tkn *token, size_t i)
{
	(void)i;
	free(token->val);
	free(token);
}

void	ast_token_clean(t_ast *ast, t_list *tokens)
{
	t_ast		*tmp_a;
	t_arr		*arr;
	unsigned	i;
	void		(*freet);

	freet = free_token;
	ft_lstdel(&tokens, freet);
	while (ast)
	{
		tmp_a = ast;
		ast = ast->down;
		if (tmp_a->next)
		{
			i = 0;
			arr = tmp_a->next;
			while (i < arr->size)
				free(arr->arr[i++]);
			free(arr->arr);
			free(arr);
		}
		free(tmp_a);
	}
}
