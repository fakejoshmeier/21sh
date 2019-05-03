/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 22:28:40 by jmeier            #+#    #+#             */
/*   Updated: 2019/04/29 08:42:43 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	bin_helper(t_list **list)
{
	t_list	*tmp;

	if (!list || !(*list))
		return ;
	tmp = *list;
	free(tmp->content);
	tmp->content = NULL;
	free(tmp);
	tmp = NULL;
}
