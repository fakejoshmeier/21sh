/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 09:04:03 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/19 17:29:40 by jmeier           ###   ########.fr       */
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

int		read_line(t_line *line, t_sh *sh)
{
	char	in;

	in = '\0';
	read(STDIN_FILENO, &in, 1);
	if (in == '\n')
	{
		line_push(line, "\0");
		write(STDOUT_FILENO, "\n", 1);
		return (TRUE);
	}
	else if (in == sh->term_settings.c_cc[VERASE] && line->length)
	{
		--line->length;
		write(STDOUT_FILENO, "\b \b", 3);
	}
	// else if ((int)in == 27)
	// 	handle_escape(line, sh->history);
	else if (in == '\t')
		autocomplete(line, sh);
	else if (ft_isprint(in))
	{
		line_push(line, &in);
		write(STDOUT_FILENO, &in, 1);
	}
	return (FALSE);
}
