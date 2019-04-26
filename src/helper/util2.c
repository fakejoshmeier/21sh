/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:40:17 by jmeier            #+#    #+#             */
/*   Updated: 2019/04/25 22:58:03 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	setshell(t_sh *sh)
{
	char		*arr[3];

	arr[0] = "setenv";
	arr[1] = "SHELL";
	arr[2] = "21sh";
	b_setenv(3, arr, sh);
	sh->history = NULL;
	sh->curr = NULL;
}

void	enter_raw_mode(void)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ECHO);
	raw.c_lflag &= ~(ICANON);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

t_list	*jm_lstnew(char *content, size_t content_size)
{
	t_list	*ret;

	ret = (t_list *)malloc(sizeof(t_list));
	ret->content = ft_strndup(content, content_size);
	ret->content_size = content_size;
	ret->next = NULL;
	return (ret);
}

t_list	*status_quo(char *path0, char *d_name, int flag)
{
	char		*path;
	struct stat	f_i;
	t_list		*ret;

	path = ft_strjoin(path0, d_name);
	stat(path, &f_i);
	if (!flag)
		ret = S_ISDIR(f_i.st_mode) ? ft_lstnew(ft_strcat(path, "/"),
		LEN(path)) : ft_lstnew(path, LEN(path));
	else
		ret = S_ISDIR(f_i.st_mode) ? ft_lstnew(ft_strcat(d_name, "/"),
		LEN(d_name)) : ft_lstnew(d_name, LEN(d_name));
	free(path);
	return (ret);
}

char	*chunk(char *str, size_t len)
{
	char	*tmp;
	char	*chr;
	char	*chr2;
	int		flag;

	if (!ft_strchr(str, '/'))
		return (str);
	flag = str[len - 1] == '/' ? 1 : 0;
	tmp = str[len - 1] == '/' ? ft_strslice(str, 0, len - 1) : str;
	chr = tmp;
	while (chr)
	{
		chr2 = ft_strchr(chr, '/');
		if (chr2)
			chr = chr2 + 1;
		else
			break ;
	}
	if (flag)
		free(tmp);
	return (chr);
}
