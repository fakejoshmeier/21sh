/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_line.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 17:21:31 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/07 13:08:44 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

t_line	*init_line(size_t size)
{
	t_line	*ret;

	ret = (t_line *)malloc(sizeof(t_line));
	ret->width = size;
	ret->capacity = 8;
	ret->length = 0;
	ret->data = malloc(sizeof(void *) * size);
	return (ret);
}

void	line_resize(t_line *line, size_t size)
{
	uint8_t	*new;

	if (size > line->length)
	{
		new = realloc(line->data, size * line->width);
		line->data = new;
		line->capacity = size;
	}
}

void	line_push(t_line *line, void *gnu)
{
	if (line->length < line->capacity)
		ft_memcpy(line->data + line->width * line->length++, gnu, line->width);
	else
	{
		line_resize(line, line->capacity * 2);
		line_push(line, gnu);
	}
}

void	*line_pop(t_line *line, void *item)
{
	if (line->length == 0)
		return (NULL);
	if (!item)
	{
		--line->length;
		return (NULL);
	}
	ft_memcpy(item, line->data + line->width * --line->length, line->width);
	return (item);
}

void	*line_get(t_line *line, size_t idx)
{
	if (!line->length || idx > line->length - 1)
		return (NULL);
	return (line->data + line->width * idx);
}
