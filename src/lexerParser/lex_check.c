/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:02:54 by jmeier            #+#    #+#             */
/*   Updated: 2019/05/20 19:25:31 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

/*
** Function pointers!  Each one is a specific check on whether something needs
** to be expanded, something needs to be escaped, something is quoted,
** something is an operator, or if it's just a plain old word.
*/

/*
** End of line check.  If not EOL, go to next check
*/

int		lex_check0(char **line, t_lexer *lex)
{
	if (**line == '\0')
	{
		write(1, "0", 1);
		if (lex->tkn_start)
			token_add(lex);
		return (1);
	}
	return (0);
}

/*
** Checks if current char can be found in an operator and the next char can be
** part of an operator as well.  If so, continues on.  Else if the next char is
** not, it creates a token.  This check runs first because the token type is set
** after checking for a single char operator, so two_char_op can fail and the
** operator can get tokenized properly.
*/

int		lex_check1(char **line, t_lexer *lex)
{
	if (lex->tkn_type == OPERATOR)
	{
		write(1, "1", 1);
		if (two_char_op(lex))
		{
			write(1, ":O", 2);
			++(lex->tkn_len);
		}
		else
		{
			token_add(lex);
			--(*line);
		}
		return (1);
	}
	return (0);
}

/*
** Same check as above but if this check passes, it makes the operator into a
** token
*/

// int		lex_check2(char **line, t_lexer *lex)
// {
// 	if (lex->tkn_type == OPERATOR && !op_check(*lex))
// 	{
// 		return (1);
// 	}
// 	return (0);
// }

/*
** Check for quotes.  Handles tokenizing entire quoted line
*/

int		lex_check2(char **line, t_lexer *lex)
{
	char	q;

	if (**line == '"' || **line == '\'')
	{
		write(1, "2", 1);
		if (!lex->tkn_start)
		{
			lex->tkn_start = *line;
			lex->tkn_type = WORD;
		}
		q = **line;
		++(lex->tkn_len);
		while (**line)
		{
			++(*line);
			++(lex->tkn_len);
			if (**line == q)
				break;
		}
		return (1);
	}
	return (0);
}

/*
** Checks first character of token for matching an operator.  Marks token
** as operator, which leads into lex_check1 on the next pass.  Either it's a
** two character operator, or it's marked off as a single token.
*/

int		lex_check3(char **line, t_lexer *lex)
{
	char	op;

	op = **line;
	if (op == '|' || op == ';' || op == '(' || op == ')' || op == '&'
	|| op == '<' || op == '>' || op == '{' || op == '}' || op == '!')
	{
		write(1, "3", 1);
		lex->tkn_type = OPERATOR;
		lex->tkn_start = *line;
		lex->tkn_len = 1;
		return (1);
	}
	return (0);
}

int		lex_check4(char **line, t_lexer *lex)
{
	if (ft_isspace(**line))
	{
		if (lex->tkn_start)
			token_add(lex);
		return (1);
	}
	return (0);
}

/*
** Checks if the current token is a word and to just keep moving forward.
*/

int		lex_check5(char **line, t_lexer *lex)
{
	(void)line;
	if (lex->tkn_type == WORD)
	{
		++(lex->tkn_len);
		return (1);
	}
	return (0);
}

/*
** If literally every other check has failed, then yes, this is a word.  I mean,
** it's an operator or it's not
*/

int		lex_check6(char **line, t_lexer *lex)
{
	write(1, "6", 1);
	lex->tkn_type = WORD;
	lex->tkn_start = *line;
	lex->tkn_len = 1;
	return (1);
}

// int		lex_check7(char **line, t_lexer *lex)
// {}

// int		lex_check8(char **line, t_lexer *lex)
// {}
