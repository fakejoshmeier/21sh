/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 23:55:10 by jmeier            #+#    #+#             */
/*   Updated: 2019/05/20 19:16:33 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

void	token_add(t_lexer *lex)
{
	t_tkn	*token;

	token = (t_tkn *)malloc(sizeof(t_tkn));
	token->val = ft_strndup(lex->tkn_start, lex->tkn_len);
	token->len = lex->tkn_len;
	token->type = lex->tkn_type;
	if (!lex->start)
	{
		lex->start = ft_lstnew((const void*)token, sizeof(t_tkn));
		lex->end = lex->start;
	}
	else
	{
		lex->end->next = ft_lstnew((const void*)token, sizeof(t_tkn));
		lex->end = lex->end->next;
	}
	lex->tkn_start = NULL;
	lex->tkn_len = 0;
	lex->tkn_type = -1;
}

int		token_split(char **line, t_lexer *lex)
{
	int			res;
	int			i;
	static int	(*checks[7])(char **line, t_lexer *lex) = {
		lex_check0, lex_check1, lex_check2, lex_check3, lex_check4,
		lex_check5, lex_check6
	};

	i = -1;
	while (++i < 7)
	{
		res = (checks[i])(line, lex);
		if (res)
			return (i);
	}
	return (0);
}

t_list	*lexer(char *line)
{
	t_lexer	lex;

	lex.tkn_start = line;
	lex.tkn_len = 0;
	lex.tkn_type = -1;
	lex.start = NULL;
	lex.end = NULL;
	while (*line)
	{
		token_split(&line, &lex);
		++line;
	}
	if (lex.tkn_start)
		token_add(&lex);
	return (lex.start);
}