/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 23:30:19 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/10 23:12:41 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

/*
** https://unix.stackexchange.com/questions/159513/what-are-the-shells-control
** -and-redirection-operators
**
** Alright, so this is a place where I want to try and identify what the heck
** each particular thing is.
** There are three different kinds of lexeme types that are relevant to me for
** this project:
** 	Operational operators: &, &&, (, ), ;, ;;, \n, |, ||
** 	Redirectional operators: <, > or >|, >>, << or <<-, <&, >&, <>
** 	Words: (There are Reserved Words, words that actually mean something to the
** 	shell, and regular-ass words)
** 21sh: The only two character operators I need to worry about are >>, <<, >&,
** <&. Keep for reference.  Edit two_char_op for 42sh
*/

/*
** I check if the current character and the next character can make up an
** operator.  This works because on the second character, I return false.
*/

int		two_char_op(t_lexer *lex)
{
	char	*tmp;
	int		ret;

	tmp = ft_strndup(lex->tkn_start, lex->tkn_len + 1);
	ret = op_parse(tmp);
	free(tmp);
	tmp = NULL;
	return (ret == -1 ? 0 : 1);
}

int		op_parse(char *token)
{
	int			i;
	static char	*ops[24] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"1", "0", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&", "<>", "<<-",
	">|" };

	i = 11;
	if (ft_strequ("|", token))
		return (PIPE);
	else if (ft_strequ("&", token) || ft_strequ(";", token))
		return (SEPARATOR);
	else if (ft_strequ("&>", token))
		return (GREATAND);
	else if (ft_strequ("&<", token))
		return (LESSAND);
	while (++i < 24)
	{
		if (ft_strequ(ops[i], token))
			return (i);
	}
	return (-1);
}

int		reserved_word(char *val)
{
	int			i;
	static char	*r_words[12] = { "if", "then", "else", "elif", "fi", "do",
	"done", "case", "esac", "while", "until", "for" };

	i = 0;
	while (i < 12)
	{
		if (ft_strequ(r_words[i], val))
			return (i + 24);
		++i;
	}
	return (WORD);
}
