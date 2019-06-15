/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 00:30:28 by jmeier            #+#    #+#             */
/*   Updated: 2019/06/15 09:41:33 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "libft.h"

enum			e_tkns
{
	WORD = 1, OPERATOR, IONUMBER, SEPARATOR, HEREDOC, REDIRECT, PIPE
};

enum			e_words
{
	ASSIGNMENT_WORD = PIPE + 1, NAME, NEWLINE, IO_NUMBER
};



enum			e_operators
{
	ANDIF = IO_NUMBER + 1, ORIF, DUB_SEMI,
	DUB_LESS, DGREAT, LESSAND, GREATAND, DIAMOND, DUB_LESSDASH,
	CLOBBER,
};

enum			e_reserved_words
{
	If = CLOBBER + 1, Then, Else, Elif, Fi, Do, Done,
	Case, Esac, While, Until, For,
	Lbrace, Rbrace, Bang,
	In
};

typedef struct		s_tkn
{
	char			*val;
	int				len;
	int				type;
}					t_tkn;

/*
** Lexer stores tokens in queue
*/

typedef struct		s_lexer
{
	int				tkn_len;
	int				tkn_type;
	char			*tkn_start;
	t_list			*start;
	t_list			*end;
}					t_lexer;

typedef struct		s_arr
{
	char			**arr;
	size_t			size;
}					t_arr;


typedef struct		s_ast
{
	int				curr;
	t_arr			*next;
	struct s_ast	*down;
}					t_ast;

t_list				*lexer(char *line);
int					token_split(char **line, t_lexer *lex);
void				token_add(t_lexer *lex);

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
int				op_parse(char *tkn);

t_ast			*sh_parse(t_ast *ast, t_tkn *token, t_sh *sh);
void			ast_token_clean(t_ast *ast, t_list *tokens);

#endif