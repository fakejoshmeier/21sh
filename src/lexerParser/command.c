/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 23:20:29 by jmeier            #+#    #+#             */
/*   Updated: 2019/05/20 19:07:20 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

/*
** Lexer does not handle $ and ~ expansions.  Will handle in parser
*/

void	command_parse(t_line *line, t_sh *sh)
{
	t_list	*tokens;
	t_list	*tmp;
	t_tkn	*token;

	if (!sh->history)
		sh->history = hist_new((char *)line->data);
	else
		sh->history = hist_add(sh->history, (char *)line->data);
	sh->curr = NULL;
	tokens = lexer((char *)line->data);
	//Create an AST out of the lexer here
	//Then run the command with the AST as input
	while (tokens)
	{
		token = tokens->content;
		ft_printf("Token: %s\nType: %i\n\n", token->val, token->type);
		tmp = tokens;
		tokens = tokens->next;
		free(token->val);
		free(token);
		free(tmp);
	}
	line->length = 0;
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

/*
** Admittedly imperfect.  ac gets larger due to closing quotes.
*/

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
