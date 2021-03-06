/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 13:46:45 by jmeier            #+#    #+#             */
/*   Updated: 2019/09/22 22:11:19 by jmeier           ###   ########.fr       */
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

char	*last_piece(char *data)
{
	char	find;
	int		idx;
	int		tmp;

	idx = ft_strrchr_ind(data, ';');
	tmp = (ft_strrchr_ind(data, '&'));
	find = tmp > idx ? '&' : ';';
	idx = tmp > idx ? tmp : idx;
	tmp = (ft_strrchr_ind(data, '|'));
	find = tmp > idx ? '|' : find;
	idx = tmp > idx ? tmp : idx;
	tmp = (ft_strrchr_ind(data, '<'));
	find = tmp > idx ? '<' : find;
	idx = tmp > idx ? tmp : idx;
	tmp = (ft_strrchr_ind(data, '>'));
	find = tmp > idx ? '>' : find;
	idx = tmp > idx ? tmp : idx;
	tmp = (ft_strrchr_ind(data, ' '));
	find = tmp > idx ? ' ' : find;
	return (ft_strrchr(data, find));
}

int		is_op(char *data, int i)
{
	if (!i)
		return (0);
	while (--i > 0)
	{
		if (ft_isalnum(data[i]))
			return (0);
		else if (data[i] == ';' || data[i] == '|')
			return (1);
		else if (data[i] == '&')
			return ((data[i - 1] == '<' || data[i - 1] == '>') ? 0 : 1);
	}
	return (0);
}
