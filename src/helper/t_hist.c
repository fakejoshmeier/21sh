/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_hist.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:25:36 by jmeier            #+#    #+#             */
/*   Updated: 2019/04/29 10:23:15 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

t_hist	*hist_new(char *content)
{
	t_hist	*ret;

	ret = (t_hist *)malloc(sizeof(t_hist));
	ret->content = ft_strdup(content);
	ret->next = NULL;
	ret->prev = NULL;
	return (ret);
}

t_hist	*hist_add(t_hist *hist, char *add)
{
	t_hist	*new;

	new = hist_new(add);
	hist->prev = new;
	new->next = hist;
	hist = new;
	return (hist);
}

/*
** Delete entire history
*/

void	hist_del(t_hist *hist)
{
	t_hist	*tmp;

	tmp = hist;
	hist = hist->next;
	tmp->prev = NULL;
	tmp->next = NULL;
	free(tmp->content);
	free(tmp);
	if (hist)
		hist_del(hist);
}
