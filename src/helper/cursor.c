/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 22:56:19 by jmeier            #+#    #+#             */
/*   Updated: 2019/04/25 22:56:44 by jmeier           ###   ########.fr       */
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
