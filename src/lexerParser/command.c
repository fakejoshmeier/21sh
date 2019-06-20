/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 23:20:29 by jmeier            #+#    #+#             */
/*   Updated: 2019/06/20 00:45:35 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

/*
** Adds command to history, even in the case of invalid commands, as zsh does.
** After command is added, the input is separated into tokens, then the word
** tokens are stored into separate arrays, separated by their operators.
** If there are parse errors, the execution stops and everything should be
** cleaned up.  For redirections, pipes, and other things that require
** two processes, there is a specialized execution set, otherwise it all
** continues as did before.
*/

//https://dev.to/oyagci/generating-a-parse-tree-from-a-shell-grammar-f1
void	command_parse(t_line *line, t_sh *sh)
{
	t_list	*tokens[2];
	t_tkn	*token;
	t_ast	*ast[2];

	sh->history = !sh->history ? hist_new((char *)line->data) :
		hist_add(sh->history, (char *)line->data);
	sh->curr = NULL;
	tokens[1] = lexer((char *)line->data);
	line->length = 0;
	tokens[0] = tokens[1];
	ast[1] = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast[0] = ast[1];
	while (tokens[1])
	{
		token = tokens[1]->content;
		if (!(ast[1] = sh_parse(ast[1], token, sh)))
		{
			ast_token_clean(ast[0], tokens[0]);
			return ;
		}
		tokens[1] = tokens[1]->next;
	}
	ast_command(ast[0], sh);
	ast_token_clean(ast[0], tokens[0]);
}

t_ast	*pipe(t_ast *ast, t_ast *down, t_sh *sh)
{
	char	*sys[2];
	int		checks[2];

	checks[0] = check_executable(ast->next->arr[0]);
	
}

/*
** The first call checks for compiled executables, eg this.
** The second call is builtin functions
** The final call is system functions like ls.
** Check for if ast->down->curr is pipe or redirect
*/


//http://www.sarathlakshman.com/2012/09/24/implementation-overview-of-redirection-and-pipe-operators-in-shell

void	ast_command(t_ast *ast, t_sh *sh)
{
	t_fptr	b_in;
	char	*sys;

	while (ast)
	{
		if (ast->down->curr == PIPE || ast->down->curr == REDIRECT)
		{
			ast = ast->down->curr == PIPE ? pipe(ast, ast->down, sh) :
			redirect();
			continue ;
		}
		if (check_executable(ast->next->arr[0]))
			execute(ast->next->arr[0], ast->next->arr, sh);
		else if ((b_in = ft_map_get(&sh->builtin,
			ft_map_hash(&sh->builtin, ast->next->arr[0]))))
			b_in(ast->next->size, ast->next->arr, sh);
		else if ((sys = ft_map_get(&sh->path, ft_map_hash(&sh->path,
			ast->next->arr[0]))))
			execute(sys, ast->next->arr, sh);
		else
			ft_printf(BLU"jo.sh"RES": command not found: %s\n",
			ast->next->arr[0]);
		ast = ast->down;
	}
}

void	execute(char *cmd, char **av, t_sh *sh)
{
	pid_t	pid;
	char	**env;

	pid = fork();
	env = NULL;
	if (pid == 0)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &sh->term_settings);
		env = map_to_array(&sh->env);
		execve(cmd, av, env);
		ft_putendl("Failed to execute command");
		free(av);
		exit(1);
	}
	else
		waitpid(pid, 0, 0);
	enter_raw_mode();
	free(env);
}

int		check_executable(char *exe)
{
	struct stat	info;

	if (!ft_strcmp(dirname(exe), ".") && ft_strncmp(exe, ".", 1))
		return (0);
	if (access(exe, X_OK))
		return (0);
	stat(exe, &info);
	if (!S_ISREG(info.st_mode))
		return (0);
	return (1);
}
