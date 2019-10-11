/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 23:55:10 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/10 23:12:23 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

t_tkn	*new_token(t_lexer *lex)
{
	t_tkn	*token;

	token = (t_tkn *)malloc(sizeof(t_tkn));
	token->val = ft_strndup(lex->tkn_start, lex->tkn_len);
	token->len = lex->tkn_len;
	token->fd = -1;
	token->type = lex->tkn_type;
	if (lex->tkn_type == IONUMBER)
		token->op_type = IONUMBER;
	else if (lex->tkn_type == WORD)
		token->op_type = reserved_word(token->val);
	else
	{
		token->op_type = op_parse(token->val);
		if (token->op_type >= LESS && token->op_type <= CLOBBER)
			token->type = REDIRECT;
	}
	token->next = NULL;
	return (token);	
}

void	token_add(t_lexer *lex)
{
	t_tkn	*token;

	token = new_token(lex);
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
	static int	(*checks[9])(char **line, t_lexer *lex) = {
		lex_check0, lex_check1, lex_check2, lex_check3,
		lex_check4, lex_check5, lex_check6, lex_check7,
		lex_check8
	};

	i = -1;
	while (++i < 9)
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

	lex.tkn_start = NULL;
	lex.tkn_len = 0;
	lex.tkn_type = -1;
	lex.start = NULL;
	lex.end = NULL;
	while (*line)
	{
		token_split(&line, &lex);
		++line;
	}
	lex.tkn_type = lex.tkn_type == IONUMBER ? WORD : lex.tkn_type;
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
