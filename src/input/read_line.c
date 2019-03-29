/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 09:04:03 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/23 15:28:56 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

// void	handle_updown(t_line *line, t_list *history, char in)
// {
// 	if (line->length == 0 && in == 'B')
// 		play_noise();
// 	else
// 	{
// 		line->length = 0;
// 		g_history = in == 'A' ? g_history->next : g_history->prev;
// 		//clear out the line;
// 		line_push(g_history->content);
// 		//get rid of the null terminator
// 		line->length = g_history->content_size;
// 	}
// }

// void	handle_escape(t_line *line, t_list *history)
// {
// 	char	in;

// 	in = '\0';
// 	read(STDIN_FILENO, &in, 1);
// 	in != '[' ? fuck() : read(STDIN_FILENO, &in, 1);
// 	if (in == 'A' || in == 'B')
// 		handle_updown(line, history, in);
// 	else if (in == 'C' || in == 'D')
// 		if (cursor_pos + 1 < line->length || cursor_pos - 1 > 0)
// 			in == 'C' ? 
// }

void	handle_write(t_line *line, char in, int del_flag)
{
	if (del_flag)
	{
		--line->length;
		write(STDOUT_FILENO, "\b \b", 3);
		return ;
	}
	in == '\n' ? line_push(line, "\0") : line_push(line, &in);
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
	else if (ft_isprint(in) || in == '\n')
		handle_write(line, in, 0);
	return (in == '\n' ? TRUE : FALSE);
}
