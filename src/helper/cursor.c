/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 22:56:19 by jmeier            #+#    #+#             */
/*   Updated: 2019/05/03 13:47:44 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	wipe_word(int len)
{
	char		*tmp;
	char		*tmp2;
	char		buf[3];

	tmp = ft_itoa(len);
	buf[0] = '\033';
	buf[1] = '[';
	buf[2] = '\0';
	tmp2 = ft_strjoin(buf, tmp);
	free(tmp);
	tmp = ft_strjoin(tmp2, "D\033[J");
	write(1, tmp, ft_strlen(tmp));
	free(tmp);
	free(tmp2);
}

void	clear_line(t_line *line, char *new)
{
	int		i;

	i = 0;
	line->length = 0;
	g_pos > 0 ? ft_printf("\033[%iD\033[J", g_pos) : 0;
	if (new)
	{
		while (new[i])
		{
			write(1, &new[i], 1);
			line_push(line, &new[i++]);
		}
		g_pos = line->length;
	}
}
