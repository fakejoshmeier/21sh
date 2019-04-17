/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 09:04:03 by jmeier            #+#    #+#             */
/*   Updated: 2019/04/16 16:05:55 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	clear_line(t_line *line, char *new)
{
	int		i;

	i = 0;
	line->length = 0;
	ft_printf("\033[%iJ", g_pos + 1);
	if (new)
	{
		while (new[i])
		{
			write(1, &new[i], 1);
			line_push(line, &new[i++]);
		}
	}
}

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

// The second condition is always true
void	handle_escape(t_line *line, t_sh *sh, char in)
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

void	handle_write(t_line *line, char in, int del_flag)
{
	if (del_flag)
	{
		--line->length;
		write(STDOUT_FILENO, "\b \b", 3);
		return ;
	}
	in == '\n' ? line_push(line, "\0") : line_push(line, &in);
	++g_pos;
	write(STDOUT_FILENO, &in, 1);
}

int		read_line(t_line *line, t_sh *sh)
{
	char	in;

	in = '\0';
	read(STDIN_FILENO, &in, 1);
	if (in == sh->term_settings.c_cc[VERASE] && line->length)
		handle_write(line, in, 1);
	else if (in == '\t' && line->length)
		return (autocomplete(line, sh));
	else if ((int)in == 27)
		handle_escape(line, sh, in);
	else if (ft_isprint(in) || in == '\n')
		handle_write(line, in, 0);
	return (in == '\n' ? TRUE : FALSE);
}
