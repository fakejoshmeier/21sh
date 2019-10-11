/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 13:52:13 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/08 21:09:03 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

extern volatile	sig_atomic_t g_running;
extern volatile	sig_atomic_t g_clear;

void	signal_handler(void)
{
	struct sigaction	action;
	void				(*act);

	act = quit;
	action = (struct sigaction){.sa_handler = act, .sa_flags = 0};
	sigemptyset(&action.sa_mask);
	sigaction(SIGTERM, &action, NULL);
	action = (struct sigaction){.sa_handler = act, .sa_flags = 0};
	sigemptyset(&action.sa_mask);
	sigaction(SIGQUIT, &action, NULL);
	act = ignore;
	action = (struct sigaction){.sa_handler = act, .sa_flags = 0};
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, NULL);
}

void	ignore(void)
{
	g_clear = TRUE;
}

void	quit(void)
{
	g_running = FALSE;
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
