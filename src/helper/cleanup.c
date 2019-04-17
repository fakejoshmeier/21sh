/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 22:28:40 by jmeier            #+#    #+#             */
/*   Updated: 2019/04/17 01:08:06 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	bin_helper(t_list **list, char **str)
{
	t_list	*tmp;

	while (*list)
	{
		free((*list)->content);
		tmp = (*list)->next;
		free(*list);
		*list = NULL;
		*list = tmp;
	}
	ft_strdel(str);
}