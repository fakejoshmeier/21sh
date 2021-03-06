/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 08:50:21 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/06 14:13:50 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

/*
** So aside from all the PATH, builtin, and env variables, I also need to have
** a thing for certain lexemes, i.e. the redirects, the pipe, etc.
** I'll just cheat and store them all in a macro.  They're a set of very single
** char operators in the first place; it's quicker to just compare them
** straightout without going through the whole hashing process.
*/

volatile	sig_atomic_t g_running = TRUE;
volatile	sig_atomic_t g_clear = FALSE;

ENVIRON;

int		handle_clear(t_line *line)
{
	if (g_clear)
		line->length = 0;
	return (g_clear);
}

void	trie_hugger(t_sh *sh)
{
	uint32_t	i;

	ft_map_init(&sh->trie, 0, 123);
	i = 48;
	while (i < 123)
		ft_map_set(&sh->trie, i++, NULL);
}

void	hash_slinging_slasher(t_sh *sh)
{
	t_cont	*tmp;
	int		i;

	ft_map_init(&sh->builtin, 0, 18);
	ft_map_init(&sh->env, 0, 99);
	ft_map_set(&sh->builtin, ft_map_hash(&sh->builtin, "echo"), &b_echo);
	ft_map_set(&sh->builtin, ft_map_hash(&sh->builtin, "cd"), &b_cd);
	ft_map_set(&sh->builtin, ft_map_hash(&sh->builtin, "env"), &b_env);
	ft_map_set(&sh->builtin, ft_map_hash(&sh->builtin, "setenv"), &b_setenv);
	ft_map_set(&sh->builtin, ft_map_hash(&sh->builtin, "unsetenv"),
		&b_unsetenv);
	ft_map_set(&sh->builtin, ft_map_hash(&sh->builtin, "exit"), &b_exit);
	i = -1;
	while (environ[++i])
	{
		tmp = (t_cont *)malloc(sizeof(t_cont));
		tmp->var = ft_strdup(environ[i]);
		tmp->value = ft_strchr(tmp->var, '=');
		*(tmp->value) = '\0';
		ft_map_set(&sh->env, ft_map_hash(&sh->env, tmp->var), tmp);
		*(tmp->value)++ = '=';
	}
	setshell(sh);
	ft_map_init(&sh->path, 0, 1080);
	update_path(sh);
}

/*
**	New prompt means that the cursor position has essentially been reset
*/

void	prompt(t_sh *sh)
{
	char	pwd[PATH_MAX];
	char	*curr;
	t_cont	*tmp;

	if (g_clear)
	{
		g_clear = FALSE;
		write(1, "\n", 1);
	}
	getcwd(pwd, PATH_MAX);
	tmp = ft_map_get(&sh->env, ft_map_hash(&sh->env, "HOME"));
	if (pwd[0] == '/' && ft_strlen(pwd) == 1)
		curr = "/";
	else if (!ft_strcmp(pwd, tmp->value))
		curr = "~";
	else
	{
		curr = ft_strrchr(pwd, '/');
		curr = curr ? curr + 1 : "?";
	}
	ft_printf(BLU"jo.sh "MAG B "%s "RES GRE"$ "RES, curr);
	g_pos = 0;
}

/*
** The core of the project is still the same as minishell.  What needs to be
** revamped specifically is my input method and my parsing, meaning the
** command suite I wrote last time is now effectively obsolete.  The same with
** the readline function. No wait, the line struct can still work.  Just need
** to noodle with it for a bit.
** Global value of cursor positioning?
*/

int		main(void)
{
	t_sh	sh;
	t_line	*line;

	tcgetattr(STDIN_FILENO, &sh.term_settings);
	enter_raw_mode();
	getcwd(sh.cwd, MAXPATHLEN);
	getcwd(sh.old, MAXPATHLEN);
	// signal_handler();
	trie_hugger(&sh);
	hash_slinging_slasher(&sh);
	line = init_line(1);
	prompt(&sh);
	while (g_running)
	{
		if ((!read_line(line, &sh)) && !handle_clear(line))
			continue ;
		line->length > 1 ? lexer_parser(line, &sh) : (line->length = 0);
		if (g_running)
			prompt(&sh);
	}
	// Make a cleanup function, dammit!
	// clean_up(&sh);
	free(line->data);
	free(line);
	return (0);
}
