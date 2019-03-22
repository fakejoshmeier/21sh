/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 13:52:13 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/19 16:48:02 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern volatile	sig_atomic_t g_running;
extern volatile	sig_atomic_t g_clear;

void	signal_handler(int signal, void (*handler))
{
	struct sigaction	action;

	action = (struct sigaction){.sa_handler = handler, .sa_flags = 0};
	sigemptyset(&action.sa_mask);
	sigaction(signal, &action, NULL);
}

void	ignore(void)
{
	g_clear = TRUE;
}

void	quit(void)
{
	g_running = FALSE;
}

void	enter_raw_mode(void)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ECHO);
	raw.c_lflag &= ~(ICANON);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

char	**sanitize_av(char **av, int *ac)
{
	char		**tmp;
	char		**av_new;
	int			ac_new;
	int			i;
	int			j;

	tmp = av;
	i = -1;
	ac_new = 0;
	while (++i < *ac)
		if (tmp[i])
			++ac_new;
	av_new = (char **)malloc(sizeof(char *) * ac_new + 1);
	av_new[ac_new] = NULL;
	i = -1;
	j = 0;
	while (++i < *ac)
		if (av[i])
			av_new[j++] = ft_strdup(tmp[i]);
	i = -1;
	while (++i < *ac)
		free(tmp[i]);
	free(tmp);
	*ac = ac_new;
	return (av_new);
}

/*
** Due to how hashing works, the links aren't in *exact* order so strduping
** based on how many links are in the map at the time is folly and I must
** instead iterate through the whole thing.  I can allocate for the size of my
** returned array though.
*/

char	**map_to_array(t_map *map)
{
	t_list		*link;
	t_cont		*tmp;
	void		**arr;
	unsigned	i;
	unsigned	j;

	arr = ft_memalloc((sizeof(char *) * map->count + 1));
	arr[map->count] = NULL;
	i = 0;
	j = 0;
	while (i < map->capacity)
	{
		if (!(link = map->data[i++]))
			continue ;
		while (link)
		{
			tmp = link->content;
			arr[j++] = ft_strdup(tmp->var);
			link = link->next;
		}
	}
	return ((char **)arr);
}
