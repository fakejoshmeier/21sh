/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 11:53:30 by jmeier            #+#    #+#             */
/*   Updated: 2019/04/29 09:38:01 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

/*
** Autocomplete notes: The shell autocomplete can detect whether something is an
** operation or operand.
** All operations come from the PATH, all operands come from the directory
** itself.
** Create a circular ll of all the PATH programs/folder progams that match the
** buffer.
*/

void	handle_switch(t_line *line, t_list *list, char in, t_sh *sh)
{
	char	*str;
	t_list	*head;
	t_list	*tmp;

	str = list->content;
	while (*str)
		line_push(line, &(*str++));
	if (in == sh->term_settings.c_cc[VERASE] && line->length)
		handle_write(line, in, 1);
	// else if ((int)in == 27)
	// 	handle_escape(line, );
	else if (ft_isprint(in))
		handle_write(line, in, 0);
	head = list;
	list = list->next;
	while (list)
	{
		if (list == head)
			break ;
		tmp = list;
		list = list->next;
		bin_helper(&tmp);
	}
	bin_helper(&head);
}

void	cycle(t_list *list, t_line *line, char *naughtocomplete, t_sh *sh)
{
	char	c;

	c = '\0';
	line->length = 0;
	if (naughtocomplete != NULL)
		while (*naughtocomplete)
			line_push(line, &(*naughtocomplete++));
	write(STDOUT_FILENO, list->content, list->content_size);
	while (list)
	{
		if (read(STDIN_FILENO, &c, 1))
		{
			if (c == '\t')
			{
				ft_printf("\033[%iD\033[J", list->content_size);
				list = list->next;
				write(STDOUT_FILENO, list->content, list->content_size);
			}
			else
				break ;
		}
	}
	handle_switch(line, list, c, sh);
}

t_list	*create_autocomplete_ll(char *buf, t_list *bin, int flag)
{
	char	*tmp;
	int		len;
	t_list	*top[2];
	t_list	*trash[2];

	NULL_GUARD(bin);
	tmp = ft_strchr(buf, '/') ? ft_strrchr(buf, '/') + 1 : buf;
	len = ft_strlen(tmp);
	top[1] = NULL;
	while (bin)
	{
		if (ft_strnequ(tmp, chunk(bin->content, bin->content_size), len))
		{
			top[0] = jm_lstnew(bin->content, bin->content_size);
			top[0]->next = top[1];
			trash[0] = !top[1] ? top[0] : trash[0];
			top[1] = top[0];
		}
		trash[1] = bin;
		bin = bin->next;
		flag ? bin_helper(&trash[1]) : 0;
		trash[1] = NULL;
	}
	top[1] == NULL ? 0 : (trash[0]->next = top[1]);
	return (top)[1];
}

/*
** Gross ass macros to beat the norme's line limits
*/

t_list	*directory_contents_to_ll(char *buf)
{
	struct dirent	*f;
	DIR				*dir;
	t_list			*ret[2];
	char			*p;
	int				flag;

	p = ft_strchr(buf, '/') ? DUG(buf, FUG(buf, '/') + 1) : ft_strdup("./");
	if (!(dir = opendir(p)))
	{
		free(p);
		return (NULL);
	}
	flag = ft_strequ("./", p);
	ret[0] = NULL;
	while ((f = readdir(dir)))
	{
		if (f->d_name[0] == '.')
			continue ;
		ret[1] = status_quo(p, f->d_name, flag);
		ret[1]->next = ret[0];
		ret[0] = ret[1];
	}
	free(p);
	closedir(dir);
	return (ret[0]);
}

/*
** I have a trie for all the possible programs in the PATH, but if the input
** is not a PATH command, I can rip the dir and get the list of stuff in the
** directory
*/

int		autocomplete(t_line *line, t_sh *sh)
{
	t_list	*bin;
	t_list	*bin2;
	char	*buf;
	char	*naughtocomplete;
	int		naughtocomplete_len;

	line_push(line, "\0");
	--line->length;
	buf = ft_strrchr((char *)line->data, ' ');
	buf = buf ? buf + 1 : (char *)line->data;
	naughtocomplete_len = ft_strlen((char *)line->data) - ft_strlen(buf);
	bin = naughtocomplete_len ? directory_contents_to_ll(buf) :
		(t_list *)ft_map_get(&sh->trie, (uint32_t)buf[0]);
	bin2 = create_autocomplete_ll(buf, bin, naughtocomplete_len);
	if (!bin2)
		return (FALSE);
	naughtocomplete = ft_strndup((char *)line->data, naughtocomplete_len);
	if (naughtocomplete_len && ft_strlen(buf))
		ft_printf("\033[%iD\033[J%s", line->length, naughtocomplete);
	else if (!naughtocomplete_len)
		ft_printf("\033[%iD\033[J", line->length);
	cycle(bin2, line, naughtocomplete, sh);
	if (naughtocomplete)
		free(naughtocomplete);
	return (TRUE);
}
