/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 22:28:40 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/22 23:18:27 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	clean_bin(t_list **list)
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
}