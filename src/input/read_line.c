/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 09:04:03 by jmeier            #+#    #+#             */
/*   Updated: 2019/05/02 22:02:48 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	handle_updown(t_line *line, t_sh *sh, char in)
{
	if (!sh->history)
		return ;
	if (in == 'B')
	{
		if (!sh->curr->prev)
			return (clear_line(line, NULL));
		sh->curr = sh->curr->prev;
	}
	else if (in == 'A')
	{
		if (!sh->curr->next)
			return ;
		sh->curr = sh->curr->next;
	}
	line->length = 0;
	clear_line(line, sh->curr->content);
}

/*
** My thoughts on g_pos are basically that in the default position, i.e. at
** the very end of the input, the index is the position of the new char that is
** yet to be appended to the end of the input.
*/

void	do_a_thing(t_line *line, char in, t_sh *sh)
{
	char	*tmp;
	char	*tmp2;
	int		move;

	line_push(line, "\0");
	g_pos > 0 ? wipe_word(g_pos) : write(1, "\33[J", 3);
	tmp = ft_strslice((char *)line->data, g_pos, (int)line->length - g_pos - 1);
	move = ft_strlen(tmp);
	line->length = g_pos;
	in == sh->term_settings.c_cc[VERASE] && line->length ? line->length-- :
		line_push(line, &in);
	in == sh->term_settings.c_cc[VERASE] ?
		--g_pos : ++g_pos;
	write(1, line->data, line->length);
	write(1, tmp, move);
	tmp2 = tmp;
	while (*tmp2)
		line_push(line, &(*tmp2++));
	free(tmp);
	in == sh->term_settings.c_cc[VERASE] ? ft_printf("\033[%iD", move) :
	ft_printf("\033[%iD", line->length - g_pos);
}

void	handle_escape(t_line *line, char in, t_sh *sh)
{
	read(STDIN_FILENO, &in, 1);
	in != '[' ? write(1, "\a", 1) : read(STDIN_FILENO, &in, 1);
	if (in == 'A' || in == 'B')
		handle_updown(line, sh, in);
	else if (in == 'C')
	{
		if (g_pos + 1 >= (int)line->length + 1)
			write(1, "\a", 1);
		else
		{
			ft_printf("\033[%c", in);
			++g_pos;
		}
	}
	else if (in == 'D')
	{
		if (g_pos - 1 < 0)
			write(1, "\a", 1);
		else
		{
			ft_printf("\033[%c", in);
			--g_pos;
		}
	}
}

void	handle_write(t_line *line, char in, t_sh *sh)
{
	if (in == sh->term_settings.c_cc[VERASE] && line->length)
	{
		if (g_pos == (int)line->length && g_pos > 0)
		{
			--line->length;
			--g_pos;
			write(STDOUT_FILENO, "\b \b", 3);
		}
		else if (g_pos > 0)
			do_a_thing(line, in, sh);
		return ;
	}
	if (in == '\n')
		line_push(line, "\0");
	else
	{
		if (g_pos == (int)line->length)
		{
			line_push(line, &in);
			++g_pos;
		}
		else
			do_a_thing(line, in, sh);
	}
	g_pos == (int)line->length || in == '\n' ? write(STDOUT_FILENO, &in, 1) : 0;
}

int		read_line(t_line *line, t_sh *sh)
{
	char	in;

	in = '\0';
	read(STDIN_FILENO, &in, 1);
	if (in == sh->term_settings.c_cc[VERASE] && line->length)
		handle_write(line, in, sh);
	else if (in == '\t' && line->length)
	{
		if (!autocomplete(line, sh))
			write(1, "\a", 1);
		return (FALSE);
	}
	else if ((int)in == 27)
		handle_escape(line, in, sh);
	else if (ft_isprint(in) || in == '\n')
		handle_write(line, in, sh);
	return (in == '\n' ? TRUE : FALSE);
}
