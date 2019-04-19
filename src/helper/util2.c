/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:40:17 by jmeier            #+#    #+#             */
/*   Updated: 2019/04/18 17:14:18 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	setshell(t_sh *sh)
{
	char	*arr[3];

	arr[0] = "setenv";
	arr[1] = "SHELL";
	arr[2] = "21sh";
	b_setenv(3, arr, sh);
	sh->history = NULL;
	sh->curr = NULL;
}

void	wipe_word(int len)
{
	char	*tmp;
	char	*tmp2;
	char	buf[3];

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