/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 09:47:49 by jmeier            #+#    #+#             */
/*   Updated: 2019/06/15 10:02:56 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

t_arr	*add_arg(t_arr *args, char *s, t_sh *sh)
{
	size_t	i;
	t_arr	*ret;

	ret = (t_arr *)ft_memalloc(sizeof(t_arr));
	ret->size = args ? args->size : 0;
	ret->arr = (char **)ft_memalloc(sizeof(char *) * (ret->size + 1));
	if (!args)
		ret->arr[0] = expand(s, sh);
	else
	{
		i = 0;
		while (i < args->size)
		{
			ret->arr[i] = ft_strdup(args->arr[i]);
			free(args->arr[i++]);
		}
		ret->arr[i] = expand(s, sh);
		free(args->arr);
		free(args);
	}
	++(ret->size);
	return (ret);
}

t_ast	*parse_error(char *val)
{
	ft_printf("jo.sh: parse error near `%s'\n", val);
	return (NULL);
}

t_ast	*sh_parse(t_ast *ast, t_tkn *token, t_sh *sh)
{
	if (token->type == OPERATOR)
	{
		if (ast->next)
		{
			ast->down = (t_ast *)ft_memalloc(sizeof(t_ast));
			ast->down->curr = op_parse(token->val);
			return (ast->down);
		}
		else if (!ast->curr && !ast->next)
		{
			ast->curr = op_parse(token->val);
			if (ast->curr == 3 || ast->curr == PIPE ||
				(ast->curr >= 12 && ast->curr <= 14))
				return (parse_error(token->val));
		}
		else if (ast->curr && !ast->next)
			return (parse_error(token->val));
	}
	else
		ast->next = add_arg(ast->next, token->val, sh);
	return (ast);
}