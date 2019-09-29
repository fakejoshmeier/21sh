/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 22:28:40 by jmeier            #+#    #+#             */
/*   Updated: 2019/09/22 22:10:52 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

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
