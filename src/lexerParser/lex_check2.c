/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_check2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:02:23 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/10 23:12:30 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

/*
** All the checks here are related to sifting out WORD operands
*/

/*
** IO Numbers are a thing, apparently.  But only in the case of a redirection.
*/

int		lex_check4(char **line, t_lexer *lex)
{
	if (**line >= '0' && **line <= '9')
	{
		if (!(lex->tkn_start))
		{
			lex->tkn_start = *line;
			lex->tkn_type = IONUMBER;
		}
		++(lex->tkn_len);
		return (1);
	}
	return (0);
}

/*
** In the case of an escaped space, the buffer is moved forward and both the
** escape and the following character are kept.  I'll parse it all as part of
** the AST creation process
*/

int		lex_check5(char **line, t_lexer *lex)
{
	char	e;

	if (**line == '\\')
	{
		e = (**line + 1);
		if (ft_isprint(e))
		{
			lex->tkn_type = WORD;
			++(*line);
			lex->tkn_len += 2;
			return (1);
		}
	}
	return (0);
}

/*
** I've got a blank space, baby.  And I'll write your name
*/

int		lex_check6(char **line, t_lexer *lex)
{
	if (ft_isspace(**line))
	{
		if (lex->tkn_start)
		{
			if (lex->tkn_type == IONUMBER && lex->end->op_type != GREATAND &&
			lex->end->op_type != LESSAND)
				lex->tkn_type = WORD;
			token_add(lex);
		}
		return (1);
	}
	return (0);
}

/*
** Checks if the current token is a word and to just keep moving forward.
*/

int		lex_check7(char **line, t_lexer *lex)
{
	if (lex->tkn_type == WORD || lex->tkn_type == IONUMBER)
	{
		if (ft_isalpha(**line) && lex->tkn_type == IONUMBER)
			lex->tkn_type = WORD;
		++(lex->tkn_len);
		return (1);
	}
	return (0);
}

/*
** If literally every other check has failed, then yes, this is a word.  I mean,
** c'mon, it's an operator or it's not.
*/

int		lex_check8(char **line, t_lexer *lex)
{
	lex->tkn_type = WORD;
	lex->tkn_start = *line;
	lex->tkn_len = 1;
	return (1);
}
