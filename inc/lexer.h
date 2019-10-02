/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 00:30:28 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/01 19:26:47 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "libft.h"
# include "sh.h"

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
	int				op_type;
	struct s_tkn	*next;
}					t_tkn;

/*
** Lexer stores tokens in queue
*/

typedef struct		s_lexer
{
	int				tkn_len;
	int				tkn_type;
	char			*tkn_start;
	t_tkn			*start;
	t_tkn			*end;
}					t_lexer;

typedef struct		s_ast
{
	struct s_ast	*parent;
	struct s_ast	*left;
	struct s_ast	*right;
	int				type;
	int				op_type;
	t_tkn			*token;
	t_tkn			*end;
}					t_ast;

/*
** Lexer body
*/

t_tkn				*lexer(char *line);
int					token_split(char **line, t_lexer *lex);
void				token_add(t_lexer *lex);

/*
** Lexer checks
*/

int					lex_check0(char **line, t_lexer *lex);
int					lex_check1(char **line, t_lexer *lex);
int					lex_check2(char **line, t_lexer *lex);
int					lex_check3(char **line, t_lexer *lex);
int					lex_check4(char **line, t_lexer *lex);
int					lex_check5(char **line, t_lexer *lex);
int					lex_check6(char **line, t_lexer *lex);
int					lex_check7(char **line, t_lexer *lex);
int					two_char_op(t_lexer *lex);
int					op_parse(char *tkn);
int					reserved_word(char *val);
void				token_del(t_tkn **token);

/*
** AST Creation, Token handling, Deletion
*/

t_ast				*create_ast(t_tkn **token);
t_ast				*create_list(t_tkn **token);
t_ast				*create_pipeline(t_tkn **token);
int					next_op(t_tkn *token, int op);

t_ast				*create_leaf(t_tkn **tkn, int delim);
t_ast				*create_node(t_ast *left, t_ast *node, t_ast *right);
void				ast_del(t_ast **ast);

t_ast				*sh_parse(t_ast *ast, t_tkn *token, t_sh *sh);
char				*expand(char *in, t_sh *sh);
void				ft_print_ast(t_ast *ast, char *side, int lvl);

/*
** Execution
*/

int					exec_command(t_ast *ast, t_sh *sh);
char				**sanitize_av(char **av, int *ac);
void				get_av_ac(char *x, char ***av, int *ac, t_sh *sh);
void				execute(char *cmd, char **av, t_sh *sh);
int					check_executable(char *exe);

int					ast_redirect(t_ast *ast, t_sh *sh);
int					exec_separator(t_ast *ast, t_sh *sh);
int					exec_andif(t_ast *ast, t_sh *sh);
int					exec_orif(t_ast *ast, t_sh *sh);
int					exec_pipe(t_ast *ast, t_sh *sh);

#endif