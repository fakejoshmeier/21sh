/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 23:55:10 by jmeier            #+#    #+#             */
/*   Updated: 2019/09/22 13:46:05 by jmeier           ###   ########.fr       */
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
	token->op_type = token->type == WORD ? reserved_word(token->val) :
		op_parse(token->val);
	token->next = NULL;
	if (!lex->start)
	{
		lex->start = token;
		lex->end = lex->start;
	}
	else
	{
		lex->end->next = token;
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
	static int	(*checks[8])(char **line, t_lexer *lex) = {
		lex_check0, lex_check1, lex_check2, lex_check3,
		lex_check4, lex_check5, lex_check6, lex_check7
	};

	i = -1;
	while (++i < 8)
	{
		res = (checks[i])(line, lex);
		if (res)
			return (i);
	}
	return (0);
}

t_tkn	*lexer(char *line)
{
	t_lexer	lex;
	t_tkn	*cap;

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
	cap = (t_tkn *)malloc(sizeof(t_tkn));
	cap->val = ft_strndup("\n", 1);
	cap->len = 1;
	cap->type = 404;
	cap->op_type = 404;
	cap->next = NULL;
	lex.end->next = cap;
	return (lex.start);
}
