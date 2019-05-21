/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 23:30:19 by jmeier            #+#    #+#             */
/*   Updated: 2019/05/20 19:31:33 by jmeier           ###   ########.fr       */
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
	ret = 0;
	if (ft_strequ(tmp, ">>") || ft_strequ(tmp, "<<") || ft_strequ(tmp, ">&")
	|| ft_strequ(tmp, "<&") || ft_strequ(tmp, "||") || ft_strequ(tmp, ">|")
	|| ft_strequ(tmp, "<>") || ft_strequ(tmp, "<<-"))
		ret = 1;
	free(tmp);
	tmp = NULL;
	return (ret);
}
