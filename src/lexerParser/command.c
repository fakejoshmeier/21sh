/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 23:20:29 by jmeier            #+#    #+#             */
/*   Updated: 2019/06/15 10:02:57 by jmeier           ###   ########.fr       */
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
	// ast_exec(ast[0], sh);
	ast_token_clean(ast[0], tokens[0]);
}

/*
** The first call checks for compiled executables, eg this.
** The second call is builtin functions
** The final call is system functions like ls.
*/

void	command_run(char *input, t_sh *sh)
{
	t_fptr	b_in;
	char	**av;
	char	*sys;
	int		ac;
	int		i;

	ac = 1;
	get_av_ac(input, &av, &ac, sh);
	av = sanitize_av(av, &ac);
	ft_strtolower(&av[0]);
	if (check_executable(av[0]))
		execute(av[0], av, sh);
	else if ((b_in = ft_map_get(&sh->builtin,
		ft_map_hash(&sh->builtin, av[0]))))
		b_in(ac, av, sh);
	else if ((sys = ft_map_get(&sh->path, ft_map_hash(&sh->path, av[0]))))
		execute(sys, av, sh);
	else
		ft_printf(BLU"jo.sh"RES": command not found: %s\n", av[0]);
	i = 0;
	while (i < ac)
		free(av[i++]);
	free(av);
}

void	get_av_ac(char *x, char ***av, int *ac, t_sh *sh)
{
	int		flag;
	int		len;
	int		i;
	int		j;

	i = -1;
	flag = -1;
	len = ft_strlen(x);
	while (++i < len)
	{
		flag = x[i] == '"' ? flag * -1 : flag;
		flag == -1 && (ft_isspace(x[i]) || x[i] == '"') ? ++*ac : 0;
		if (flag == -1 && (ft_isspace(x[i]) || x[i] == '"'))
			x[i] = '\0';
	}
	*av = (char **)ft_memalloc(sizeof(char *) * *ac);
	i = -1;
	j = -1;
	while (++i < *ac && flag == -1 && ++j < len)
		if (x[j])
		{
			(*av)[i] = x[j] == '"' ? ft_strdup(x + j + 1) : expand(x + j, sh);
			while (x[j])
				++j;
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
