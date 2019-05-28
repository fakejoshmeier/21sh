/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 00:30:28 by jmeier            #+#    #+#             */
/*   Updated: 2019/05/23 20:00:30 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "libft.h"

enum			e_tkns
{
	WORD = 1, OPERATOR, IONUMBER, SEPARATOR, HEREDOC, REDIRECT, PIPE
};

/*
** Word is already 1
*/

enum			e_words
{
	ASSIGNMENT_WORD = 2, NAME, NEWLINE, IO_NUMBER
};

enum			e_operators
{
	ANDIF = IO_NUMBER + 1, ORIF, DSEMI,
	DLESS, DGREAT, LESSAND, GREATAND, LESSGREAT, DLESSDASH,
	CLOBBER,
};

enum			e_reserved_words
{
	If = CLOBBER + 1, Then, Else, Elif, Fi, Do, Done,
	Case, Esac, While, Until, For,
	Lbrace, Rbrace, Bang,
	In
};


typedef struct	s_tkn
{
	char		*val;
	int			len;
	int			type;
}				t_tkn;

/*
** Lexer stores tokens in queue
*/

typedef struct	s_lexer
{
	int			tkn_len;
	int			tkn_type;
	char		*tkn_start;
	t_list		*start;
	t_list		*end;
}				t_lexer;

t_list			*lexer(char *line);
int				token_split(char **line, t_lexer *lex);
void			token_add(t_lexer *lex);

/*
** Lexer checks on the line
*/

int				lex_check0(char **line, t_lexer *lex);
int				lex_check1(char **line, t_lexer *lex);
int				lex_check2(char **line, t_lexer *lex);
int				lex_check3(char **line, t_lexer *lex);
int				lex_check4(char **line, t_lexer *lex);
int				lex_check5(char **line, t_lexer *lex);
int				lex_check6(char **line, t_lexer *lex);
int				lex_check7(char **line, t_lexer *lex);


int				two_char_op(t_lexer *lex);

// int				lex_check7(char **line, t_lexer *lex);
// int				lex_check8(char **line, t_lexer *lex);
// int				lex_check9(char **line, t_lexer *lex);

#endif